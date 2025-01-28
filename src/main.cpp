#include <windows.h>

#include "Drautos.h"
#include "Logging/Exception.h"

bool APIENTRY DllMain(
    HMODULE hModule,
    const DWORD ul_reason_for_call,
    LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        // Run Drautos code with exception handling
        // Exceptions that occur in the game code due to our changes will not be
        // caught by this However, exceptions in detour function code can be
        // caught if wrapped in CPPTRACE_TRY/CPPTRACE_CATCH
        try
        {
            Drautos::Run();
        }
        catch (...)
        {
            Exception::Fatal();
        }
    }

    return true;
}
