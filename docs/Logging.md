# Logging

Drautos uses [spdlog](https://github.com/gabime/spdlog) to log to a file, and optionally to a console
if the `EnableConsole` flag is set in the configuration.

Currently, the log file is located at `%LOCALAPPDATA%/Flagrum/logs/game_{TIMESTAMP}.log`.

## Log Levels

The following log levels should be used when logging. They are ordered by importance, with the least important first.

| Logging Level | Availability    | Description                                                                        |
|---------------|-----------------|------------------------------------------------------------------------------------|
| Trace         | Debug           | Information that can be useful to back-trace certain events.                       |
| Debug         | Debug           | Information relevant to debugging the library.                                     |
| Info          | Debug + Release | Information that helps understand the current state of the application.            |
| Warn          | Debug + Release | Warns of unexpected state or unexpected events.                                    |
| Error         | Debug + Release | Logs that an error occurred, but the application will continue to run.             |
| Critical      | Debug + Release | Logs that an unrecoverable error occurred, and the application will be terminated. |

> [!TIP]  
> For logging exceptions, see [exceptions](Exceptions.md).


**IMPORTANT**

- Do not log with trace or debug if the information would help diagnose bugs reported by users
- After logging with fatal, you must terminate the application yourself

## Logging Messages

Prefer to use the `SPDLOG` macros, as these enable the ability to display extra information such as file and line number
to the logged messages. For example:

```c++
patchManager.ApplyPatches();
SPDLOG_INFO("Patches applied");
```