#ifndef STEAMRESTARTHOOK_H
#define STEAMRESTARTHOOK_H

#include "../ExternFunctionHook.h"

namespace Hooks
{
constexpr char STEAM_RESTART_HOOK_MODULE[] = "steam_api64.dll";
constexpr char STEAM_RESTART_HOOK_FUNCTION[] = "SteamAPI_RestartAppIfNecessary";

/**
 * Prevents the Steam API from signalling the game to restart.
 * @remarks This can happen due to an issue with Steam not recognizing the app
 *          ID of the game when launched outside of Steam itself. The hook
 *          simply prevents this behaviour so the game doesn't relaunch itself
 *          without Drautos injected.
 */
class SteamRestartHook final
    : public ExternFunctionHook<STEAM_RESTART_HOOK_MODULE,
                                STEAM_RESTART_HOOK_FUNCTION, bool, uint32_t>
{
public:
    bool ShouldApply() override
    {
        return true;
    }

protected:
    /**
     * Determines whether the game should restart.
     * @param appId The Steam ID of the FFXV application.
     * @return True if the game should restart, otherwise false.
     */
    bool Detour(const uint32_t appId) override
    {
        original_(appId);
        return false;
    }
};
} // namespace Hooks

#endif // STEAMRESTARTHOOK_H
