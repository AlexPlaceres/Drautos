#ifndef STEAMRESTARTHOOK_H
#define STEAMRESTARTHOOK_H
#include "../FunctionHook.h"
#include <spdlog/spdlog.h>

namespace Hooks
{
/**
 * Prevents the Steam API from signalling the game to restart.
 * @remarks This can happen due to an issue with Steam not recognizing the app
 * ID of the game when launched outside of Steam itself. The hook simply
 * prevents this behaviour so the game doesn't relaunch itself without Drautos
 * injected.
 */
class SteamRestartHook final : public FunctionHook<0, 0, bool, uint32_t>
{
protected:
    /**
     * Determines whether the game should restart.
     * @param appId The Steam ID of the FFXV application.
     * @return True if the game should restart, otherwise false.
     */
    bool Detour(const uint32_t appId) override
    {
        SPDLOG_DEBUG("SteamAPI_RestartAppIfNecessary detour called");
        original_(appId);
        return false;
    }

public:
    bool ShouldApply() override
    {
        return true;
    }

    void** GetTargetFunctionPointerReference() override
    {
        const auto steam = GetModuleHandleA("steam_api64.dll");
        const auto address =
            GetProcAddress(steam, "SteamAPI_RestartAppIfNecessary");
        original_ =
            reinterpret_cast<Original_t>(reinterpret_cast<uint64_t>(address));
        return &reinterpret_cast<void*&>(original_);
    }
};
} // namespace Hooks

#endif // STEAMRESTARTHOOK_H
