#ifndef HOOKMANAGER_H
#define HOOKMANAGER_H

#include <detours/detours.h>
#include <processthreadsapi.h>
#include <vector>

#include "IFunctionHook.h"

namespace Hooks
{
/**
 * Manages function hooks for Drautos.
 */
class FunctionHookManager
{
private:
    std::vector<IFunctionHook*> hooks_;

    FunctionHookManager() = default;

    ~FunctionHookManager()
    {
        for (const auto hook : hooks_)
        {
            delete hook;
        }
    }

public:
    static FunctionHookManager& GetInstance()
    {
        static FunctionHookManager instance;
        return instance;
    }

    FunctionHookManager(const FunctionHookManager&) = delete;

    FunctionHookManager& operator=(const FunctionHookManager&) = delete;

    /**
     * Registers a function hook with the hook manager.
     * @tparam THook Class type of the function hook to register.
     */
    template <typename THook> void Register()
    {
        hooks_.push_back(new THook());
    }

    /**
     * Applies all registered function hooks to the game.
     */
    void ApplyHooks() const
    {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        for (const auto hook : hooks_)
        {
            if (hook->ShouldApply())
            {
                DetourAttach(hook->GetTargetFunctionPointerReference(),
                             hook->GetDetourFunctionPointer());
            }
        }

        DetourTransactionCommit();
    }
};
} // namespace Hooks

#endif // HOOKMANAGER_H
