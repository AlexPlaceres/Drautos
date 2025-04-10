#ifndef ANSELPATCH_H
#define ANSELPATCH_H

#include "../IPatch.h"

#include "../../Configuration.h"

namespace Patches
{
/**
 * Patches the game so that NVIDIA Ansel can be activated without restriction.
 * @remarks Normally, Ansel cannot be activated in cutscenes, menus, and other
 * such times. This patch alters this behaviour so it is always available.
 */
class AnselPatch final : public IPatch
{
public:
    bool ShouldApply() override
    {
        if (Configuration::GetInstance().EnableAnselPatch)
        {
            // Need to check if the patch is already applied since an old
            // executable was distributed by another developer years ago to
            // permanently modify the executable with this patch

            // Check if the target signature exists
            auto target = MemorySignature(GetTargetSignature());
            const auto matches = target.Find();
            if (matches.empty())
            {
                // Target signature does not exist, check if already patched
                auto patch = MemorySignature(GetPatchSignature());
                const auto patchMatches = patch.Find();
                if (patchMatches.empty())
                {
                    // Patch is not present, this is an error
                    Exception::Fatal("Could not apply Ansel patch "
                                     "as signature was not found.");
                }

                // Already patched, do not apply
                return false;
            }

            // Executable not modified, apply the patch
            return true;
        }

        return false;
    }

    int GetExpectedTargetCount() override
    {
        return 1;
    }

    /**
     * Targets the following pattern:
     * @code
     * 72 B3            jb  short loc_?
     * 80 7C 24 48 00   cmp [rsp+0x28+arg_18], 0
     * 75 08            jnz 0xA
     * @endcode
     * @remarks This will only jump to the Ansel session startup if the
     * condition is met as determined by the second two lines.
     */
    const char* GetTargetSignature() override
    {
        return "72 ?? 80 7C 24 48 00 75";
    }

    /**
     * Replaces with the following pattern:
     * @code
     * 72 B3            jb  short loc_?
     * 80 7C 24 48 00   cmp [rsp+0x28+arg_18], 0
     * EB 08            jmp 0xA
     * @endcode
     * @remarks This removes the condition from the jump, so the Ansel session
     * will always start.
     */
    const char* GetPatchSignature() override
    {
        return "72 ?? 80 7C 24 48 00 EB";
    }
};
} // namespace Patches

#endif // ANSELPATCH_H
