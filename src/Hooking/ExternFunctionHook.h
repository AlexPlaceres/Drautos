#ifndef EXTERNFUNCTIONHOOK_H
#define EXTERNFUNCTIONHOOK_H
#include "IFunctionHook.h"

#include <cstdint>

namespace Hooks
{
/**
 * Represents a detour that hooks an existing function from an external
 * module to alter its logic.
 * @tparam TargetModule Name of the module that contains the target function.
 * @tparam TargetFunction Name of the function that is to be hooked.
 * @tparam TReturn Return type of the target function.
 * @tparam TParams Parameter types of the target function.
 * @remarks Each implementation of this class can only be instantiated once due
 *          to the static nature of hooks.
 */
template <const char* TargetModule, const char* TargetFunction,
          typename TReturn, typename... TParams>
class ExternFunctionHook : public IFunctionHook
{
private:
    inline static ExternFunctionHook* instance_;

    /**
     * Static wrapper for @code Detour @endcode as the non-static function
     * cannot be referenced.
     * @param params The parameters to pass to @code Detour @endcode.
     * @return The return value of @code Detour @endcode.
     */
    static TReturn DetourFunction(TParams... params)
    {
        return instance_->Detour(params...);
    }

protected:
    using Original_t = TReturn (*)(TParams...);

    /**
     * Pointer to the target function. Should be invoked to call the target
     * function from the detour if the original logic should still be executed.
     */
    Original_t original_;

    /**
     * Logic to execute when detouring the target function.
     * @param params The parameters that are passed to the target function.
     * @return The return value of the target function.
     */
    virtual TReturn Detour(TParams... params) = 0;

public:
    /**
     * Instantiates the function hook.
     * @exception exception Thrown if this hook has already been instantiated
     * once.
     */
    ExternFunctionHook()
    {
        if (instance_)
        {
            Exception::Fatal("Cannot instantiate the same hook twice.");
        }

        instance_ = this;

        const auto module = GetModuleHandleA(TargetModule);
        const auto function = GetProcAddress(module, TargetFunction);
        const auto address = reinterpret_cast<uint64_t>(function);
        original_ = reinterpret_cast<Original_t>(address);
    }

    /**
     * @copydoc IFunctionHook::GetTargetFunctionPointerReference
     */
    void** GetTargetFunctionPointerReference() override
    {
        return &reinterpret_cast<void*&>(original_);
    }

    /**
     * @copydoc IFunctionHook::GetDetourFunctionPointer
     */
    void* GetDetourFunctionPointer() override
    {
        return DetourFunction;
    }
};
} // namespace Hooks

#endif // EXTERNFUNCTIONHOOK_H
