#ifndef IFUNCTIONHOOK_H
#define IFUNCTIONHOOK_H

namespace Hooks
{
/**
 * Represents a detour that hooks an existing function to add extra logic to it.
 */
class IFunctionHook
{
  public:
    virtual ~IFunctionHook() = default;

    /**
     * Whether the hook should be applied to the game or not.
     * @return True if the hook should be applied.
     */
    virtual bool ShouldApply() = 0;

    /**
     * Gets a reference to the pointer that points to the function that will be
     * detoured.
     * @return Reference to the target function pointer.
     */
    virtual void** GetTargetFunctionPointerReference() = 0;

    /**
     * Gets a pointer that points to the function that detours the target
     * function.
     * @return Pointer to the detour function.
     */
    virtual void* GetDetourFunctionPointer() = 0;
};
} // namespace Hooks

#endif // IFUNCTIONHOOK_H
