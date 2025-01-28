#ifndef HOST_H
#define HOST_H

#include "Configuration.h"

#include <cstdint>
#include <psapi.h>
#include <windows.h>

/**
 * Discerns between two relative virtual addresses based on the host process
 * type.
 * @param debugRva RVA to use if injected into the debug executable.
 * @param releaseRva RVA to use if injected into the release executable.
 * @return The resolved RVA to use.
 */
#define DISCERN(debugRva, releaseRva)                                          \
    (Host::Type == Configuration::DEBUG ? debugRva : releaseRva)

/**
 * Converts a relative virtual address (RVA) to an absolute address.
 * @param debugRva RVA to use if injected into the debug executable.
 * @param releaseRva RVA to use if injected into the release executable.
 * @return The computed absolute address.
 */
#define REBASE(debugRva, releaseRva)                                           \
    (Host::BaseAddress +                                                       \
     (Host::Type == Configuration::DEBUG ? debugRva : releaseRva))

/**
 * Assists in interacting with the host process (FFXV).
 */
class Host
{
public:
    /**
     * Handle to the host process module.
     */
    inline static HINSTANCE hModule;

    /**
     * Base address of the host process.
     */
    inline static uint64_t BaseAddress;

    /**
     * Size of the host process module, in bytes.
     */
    inline static uint32_t ModuleSize;

    /**
     * The type of game executable that Drautos was injected into.
     */
    inline static Configuration::GameExecutableType Type;

    Host() = delete;

    /**
     * Initializes this class.
     */
    static void Initialize()
    {
        hModule = GetModuleHandle("ffxv_s.exe");
        BaseAddress = reinterpret_cast<uint64_t>(hModule);
        Type = Configuration::GetInstance().HostType;

        MODULEINFO moduleInfo;
        if (!GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo,
                                  sizeof(moduleInfo)))
        {
            Exception::Fatal(
                "Failed to get module information for ffxv_s.exe.");
        }

        ModuleSize = moduleInfo.SizeOfImage;
    }
};

#endif // HOST_H
