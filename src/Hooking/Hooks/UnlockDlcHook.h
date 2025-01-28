#ifndef UNLOCKDLCHOOK_H
#define UNLOCKDLCHOOK_H
#include "../FunctionHook.h"

#include "../../Configuration.h"

namespace Hooks
{
/**
 * Unlocks all non-paid bonus content that cannot otherwise be unlocked
 * in-game.\n\n Hooks the following method:
 * @code
 * void Black::Platform::AppContent::AppContentManagerBase::ClearDLC(
 *     Black::Platform::AppContent::AppContentManagerBase* this,
 *     SQEX::Ebony::Std::DynamicArray<bool,2,16>& arg2)
 * @endcode
 */
class UnlockDlcHook final
    : public FunctionHook<0x7BCFF0, 0x7114F80, void, void*, void*>
{
public:
    bool ShouldApply() override
    {
        return Configuration::GetInstance().UnlockAdditionalDlc;
    }

protected:
    /**
     * Disables all DLC.
     * @param pAppContentManager The DLC manager.
     * @param pAppContentFlags An array of booleans representing whether each
     * DLC is enabled.
     * @remarks The original function runs before detecting and setting enabled
     * DLCs. This detour allows it to run to allocate memory for the flags, but
     * then sets them all to true to enable the content.
     */
    void Detour(void* pAppContentManager, void* pAppContentFlags) override
    {
        original_(pAppContentManager, pAppContentFlags);

        try
        {
            // This is a function that is needed to get the argument for
            // GetInstance
            using GetInstanceArg0 = void*();
            const auto getInstanceArg0 =
                reinterpret_cast<GetInstanceArg0*>(REBASE(0, 0x72cb690));

            // This is a function that gets an instance of the SaveData
            using GetInstance = void*(void*);
            const auto getInstance =
                reinterpret_cast<GetInstance*>(REBASE(0, 0x72cec10));

            // This gets the save data instance
            const auto saveData = getInstance(getInstanceArg0());

            // This iterates the SaveData->appContent flags, which are the DLC
            // flags, and sets them all to 1
            void* currentFlag = static_cast<char*>(saveData) + 0x110;
            for (auto i = 0; i < 100; i++)
            {
                *static_cast<int8_t*>(currentFlag) = 1;
                currentFlag = static_cast<char*>(currentFlag) + 1;
            }
        }
        catch (...)
        {
            Exception::Fatal("Failed to unlock hidden content");
        }
    }
};
} // namespace Hooks

#endif // UNLOCKDLCHOOK_H
