#ifndef HOST_H
#define HOST_H

#include <cstdint>
#include <exceptions.hpp>
#include <psapi.h>
#include <stdexcept>
#include <windows.h>

/**
 * Discerns between two relative virtual addresses based on the host process
 * type.
 * @param debugRva RVA to use if injected into the debug executable.
 * @param releaseRva RVA to use if injected into the release executable.
 * @return The resolved RVA to use.
 */
#define DISCERN(debugRva, releaseRva)                                          \
    (Host::Type == Host::DEBUG ? debugRva : releaseRva)

/**
 * Converts a relative virtual address (RVA) to an absolute address.
 * @param debugRva RVA to use if injected into the debug executable.
 * @param releaseRva RVA to use if injected into the release executable.
 * @return The computed absolute address.
 */
#define REBASE(debugRva, releaseRva)                                           \
    (Host::BaseAddress + (Host::Type == Host::DEBUG ? debugRva : releaseRva))

/**
 * Assists in interacting with the host process (FFXV).
 */
class Host
{
public:
    /**
     * Represents the type of game build that Drautos was injected into.
     */
    enum BuildType
    {
        UNKNOWN, /**< Release type could not be discerned. */
        DEBUG,   /**< Only known debug release of the game. */
        RELEASE  /**< Latest Steam release of the game. */
    };

    /**
     * The build type of the host process.
     */
    inline static BuildType Type;

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

    Host() = delete;

    /**
     * Initializes this class.
     */
    static void Initialize()
    {
        Type = RELEASE;
        hModule = GetModuleHandle("ffxv_s.exe");
        BaseAddress = reinterpret_cast<uint64_t>(hModule);

        MODULEINFO moduleInfo;
        if (!GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo,
                                  sizeof(moduleInfo)))
        {
            throw cpptrace::runtime_error(
                "Failed to get module information for ffxv_s.exe");
        }

        ModuleSize = moduleInfo.SizeOfImage;
    }
};

#endif // HOST_H
