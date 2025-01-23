#ifndef TWITCHPRIMEPATCH_H
#define TWITCHPRIMEPATCH_H
#include "../IPatch.h"

#include "../../Configuration.h"

namespace Patches
{
/**
 * Patches the release build of the game to enable the old Twitch Prime chocobo
 * skin.
 * @remarks This targets the setter for the DLC flags in the save data struct.
 *          While this patch is named for the Twitch Prime chocobo, it also
 * enables other DLC flags that are set in various places. However, this only
 * seems relevant to Twitch Prime as it was the only minor piece of content that
 * was not enabled by the UnlockDlcHook, while this patch does not enable all of
 * the content that that hook does.
 */
class TwitchPrimePatch final : public IPatch
{
public:
    /**
     * Only applies to the release build, as the DLC flags do not have a setter
     * in debug.\n Instead, the values of the DLC flags are assigned directly.
     */
    bool ShouldApply() override
    {
        return Host::Type == Configuration::DEBUG &&
               Configuration::GetInstance().UnlockAdditionalDlc;
    }

    int GetExpectedTargetCount() override
    {
        return 1;
    }

    /**
     * Targets the following pattern:
     * @code
     * 48 63 C2                     movsxd      rax,edx
     * 44 88 84 08 10 01 00 00      mov byte    [rax+rcx+0x110], r8b
     * C3                           ret
     * @endcode
     * @remarks This is setting the value of the given DLC index (rax) to the
     * given boolean (char) value (r8b). This is separate to UnlockDlcHook as it
     * runs separately after the function that that hooks.
     */
    const char* GetTargetSignature() override
    {
        return "48 63 C2 44 88 84 08 10 ?? ?? ?? C3";
    }

    /**
     * Replaces with the following pattern:
     * @code
     * 48 63 C2                     movsxd      rax,edx
     * C6 84 08 10 01 00 00 01      mov byte    [rax+rcx+0x110], 0x1
     * C3                           ret
     * @endcode
     * @remarks This is setting the value to 1 instead of the parameter at r8b.
     *          Essentially, this means the DLC will always be enabled.
     */
    const char* GetPatchSignature() override
    {
        return "48 63 C2 C6 84 08 10 01 00 00 01 C3";
    }
};
} // namespace Patches

#endif // TWITCHPRIMEPATCH_H
