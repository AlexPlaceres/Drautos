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
        return Configuration::GetInstance().EnableAnselPatch;
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
        return "72 ?? B3 80 7C 24 48 00 75";
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
        return "7C 24 48 00 EB 08";
    }
};
} // namespace Patches

#endif // ANSELPATCH_H
