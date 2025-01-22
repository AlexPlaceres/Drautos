﻿#ifndef FUNCTIONHOOK_H
#define FUNCTIONHOOK_H

#include <cstdint>
#include <exception>
#include <exceptions.hpp>

#include "IFunctionHook.h"

#include "../Host.h"

namespace Hooks
{
/**
 * Represents a detour that hooks an existing function to add extra logic to it.
 * @tparam TargetRvaDebug Relative virtual address of the function to hook when
 *                        injected into debug executable.
 * @tparam TargetRvaRelease Relative virtual address of the function to hook
 *                          when injected into release executable.
 * @tparam TReturn Type of the return value of the target function.
 * @tparam TParams Types of the parameters of the target function.
 * @remarks Each implementation of this class can only be instantiated once due
 *          to the static nature of hooks.
 */
template <uint64_t TargetRvaDebug, uint64_t TargetRvaRelease, typename TReturn,
          typename... TParams>
class FunctionHook : public IFunctionHook
{
private:
    inline static FunctionHook* instance_;

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
    FunctionHook()
    {
        if (instance_)
        {
            throw cpptrace::runtime_error(
                "Cannot instantiate the same hook twice.");
        }

        instance_ = this;
        original_ = reinterpret_cast<Original_t>(
            REBASE(TargetRvaDebug, TargetRvaRelease));
    }

    FunctionHook(const FunctionHook&) = delete;

    FunctionHook& operator=(const FunctionHook&) = delete;

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

#endif // FUNCTIONHOOK_H
