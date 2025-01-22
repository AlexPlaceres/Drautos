# Exception Handling

> [!CAUTION]  
> Do **not** catch any exceptions if you do not intend to handle them properly.  
> It is better to let it bubble up or crash the game to prevent users reporting unexpected behaviour as mod bugs.

## Fatal Exceptions

### What a fatal exception is

A fatal exception is an exception that would leave the game or Drautos in an unknown state, potentially causing
unexpected behaviour.

> [!WARNING]  
> It is critical that fatal exceptions are handled properly to prevent users reporting bugs
> that we have no way to diagnose.

For example, if the snapshot limit hook fails to apply to the game, unexpected behaviour will occur as the user
is expecting their snapshot limit to increase. They may report that the mod is broken, but unless we handle this
exception accordingly, we wouldn't know that it was due to the hook failing to apply.

### How to handle a fatal exception

The exception must be caught and logged, and the user needs to be informed of this so they know that there is
a log they can attach to their bug report. Further to this, it is much easier for us to work with if we have
a stack trace. How this should be done depends on where the code is executing.

#### Initialization code

This refers to any code that executes in Drautos before `DllMain` returns. This does not include any code that
is executed on a separate thread, as this will not be caught by `DllMain`'s try/catch block.

For unhandled exceptions, nothing needs to be done, `DllMain` already has this covered. For events that will leave
the DLL in an exceptional state, throw one of `cpptrace`'s exception types. These exceptions capture the stack
trace and will help greatly with diagnosing bugs.

```c++
hMappedFile = OpenFileMapping(FILE_MAP_READ, false, "DrautosConfiguration");
if (!hMappedFile)
{
    // Cannot continue without a configuration to work with
    // We cannot decide on default values as this will cause behaviour that the user is not expecting
    throw cpptrace::runtime_error("Failed to open configuration.");
}
```

#### Other code

This refers to code that is executed on other threads, or that will be executed by the game itself (such as the
code in our detour functions).

We cannot seem to use a global exception handler such as `std::set_terminate` from the injected DLL. As such, any
exceptions will either crash the game, or just prevent the DLL from continuing without any way to know that an
exception occurred. If there is a possibility for an exception to occur, wrap the code with a try/catch block.
The `CPPTRACE_TRY` macro should be used to ensure the stack trace is captured in the event that the wrapped code
does not throw `cpptrace` exceptions.

```c++
CPPTRACE_TRY
{
    auto patchIndexId = LmAssetID::Create("data://patch/patch1/patchindex.ebex@");
    SQEX::Luminous::AssetManager::LmFileList::AddPatchIndexEarc(&patchIndexId);
}
CPPTRACE_CATCH (const std::exception& exception)
{
    // The mod loader won't work without this, best to abort so the user can send us a useful log
    Exception::Fatal(exception, "Failed to add patch1 to asset manager");
}
CPPTRACE_CATCH_ALT (...)
{
    // CPPTRACE_CATCH_ALT must be used if a second catch statement is needed, just pray you don't need a third!
    Exception::Fatal("Failed to add patch1 to asset manager");
}
```

In the above example, if an exception occurs, the following happens:

* The message "Failed to add patch1 to asset manager" is logged
* The exception message is logged if the exception is a C++ exception (`std::exception` and derived types)
* The stack trace is logged
* If launched with debug settings, the console will wait until the user presses a key so they have time to read
* If launched with release settings, a message box will be displayed so the user knows what happened
* The application is terminated (thus shutting down the game)


## Non-Fatal Exceptions

### What a non-fatal exception is

A non-fatal exception is an exception that can be recovered from without causing unexpected behaviour.

> [!NOTE]  
> Non-fatal exceptions should still be logged. Use `SPDLOG_ERROR` if the exception is more serious,
> otherwise use `SPDLOG_WARN`.

For example, if reading a file to check what version of Flagrum the user has, we do not need to terminate
the application as this is purely informational and will not affect the behaviour the user expects from
the game or the mod loader, assuming this version is not used to alter functionality of the DLL or game.

```c++
try
{
    GetFlagrumVersion();
}
catch (const std::exception& exception)
{
    SPDLOG_WARN("Failed to read Flagrum version information.");
}
```