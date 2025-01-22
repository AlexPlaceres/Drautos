#include <windows.h>

#include "Drautos.h"
#include "Logging/Exception.h"
#include "Logging/Logging.h"

bool APIENTRY DllMain(HMODULE hModule, const DWORD ul_reason_for_call,
                      LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        // Setup logging
        try
        {
            Logging::Initialize();
        }
        catch (...)
        {
            // Logging and exception handling not setup yet, so this is the best
            // we can do here
            MessageBox(nullptr, "Failed to initialize mod loader.", "Error",
                       MB_OK);
            std::_Exit(EXIT_FAILURE);
        }

        // Run Drautos code with exception handling
        // Exceptions that occur in the game code due to our changes will not be
        // caught by this However, exceptions in detour function code can be
        // caught if wrapped in CPPTRACE_TRY/CPPTRACE_CATCH
        CPPTRACE_TRY
        {
            Drautos::Run();
        }
        CPPTRACE_CATCH(const std::exception& exception)
        {
            if (const auto cpptraceException =
                    dynamic_cast<const cpptrace::exception*>(&exception))
            {
                Exception::Fatal(
                    *cpptraceException,
                    "An error occurred during the operation of Drautos");
            }
            else
            {
                Exception::Fatal(
                    exception,
                    "An error occurred during the operation of Drautos");
            }
        }
        CPPTRACE_CATCH_ALT(...)
        {
            Exception::Fatal(
                "An error occurred during the operation of Drautos");
        }
    }

    return true;
}
