#ifndef IPATCH_H
#define IPATCH_H

namespace Patches
{
/**
 * Represents a pattern of memory that overwrites an existing pattern of memory.
 */
class IPatch
{
public:
    virtual ~IPatch() = default;

    /**
     * Whether the patch should be applied to the game or not.
     * @return True if the hook should be applied, otherwise false.
     */
    virtual bool ShouldApply() = 0;

    /**
     * Gets the expected number of times that the target signature should appear
     * in memory.
     * @return -1 if the number of occurrences may be variable, otherwise the
     * expected number of occurrences.
     * @remarks Must not return 0.
     */
    virtual int GetExpectedTargetCount() = 0;

    /**
     * Gets the byte pattern that this patch will replace.
     * @return The hexadecimal string representation of the target memory
     * pattern.
     */
    virtual const char* GetTargetSignature() = 0;

    /**
     * Gets the byte pattern that will replace the target signature.
     * @return The hexadecimal string representation of the patch memory
     * pattern.
     */
    virtual const char* GetPatchSignature() = 0;
};
} // namespace Patches

#endif // IPATCH_H
