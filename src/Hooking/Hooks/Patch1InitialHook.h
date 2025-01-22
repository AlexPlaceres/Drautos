#ifndef PATCH1INITIALHOOK_H
#define PATCH1INITIALHOOK_H
#include <mutex>

#include "../FunctionHook.h"

#include "../../Replica/SQEX/Luminous/AssetManager.h"
#include "../../Replica/SQEX/Luminous/AssetManager/LmAssetID.h"
#include "../../Replica/SQEX/Luminous/AssetManager/LmFileList.h"

using LmAssetID = SQEX::Luminous::AssetManager::LmAssetID;

namespace Hooks
{
/**
 * Tells the game to load mod files via the patch system.\n\n
 * Hooks the following method:
 * @code
 * int64_t Black::Main::ApplicationBase::InitializeBefore(void* applicationBase)
 * @endcode
 * @remarks This is one of two hooks that adds a patch archive to the asset
 * manager. This one is hooked earlier in the game's initialization process, as
 * calling it later prevents it from acting on assets that are loaded very early
 * on.
 */
class Patch1InitialHook final
    : public FunctionHook<0x4FBA60, 0x4CE960, int64_t, void*>
{
  private:
    using AcquireAsset_t = void*(__fastcall*)(void* pAssetManager, LmAssetID*,
                                              uint64_t, int64_t);

    std::once_flag hasApplied_;

  protected:
    /**
     * Runs early initialization steps.
     * @param pApplicationBase The game's main application class.
     * @return The return value is unknown without further investigation.
     * @remarks This detour will add patch/patch1_initial/patchindex.earc to the
     * asset manager on first call.
     */
    int64_t Detour(void* pApplicationBase) override
    {
        std::call_once(hasApplied_, [] {
            SPDLOG_INFO("Adding patch1_initial to asset manager");

            CPPTRACE_TRY
            {
                // Add patch1_initial to the asset manager
                auto patchIndexId = LmAssetID::Create(
                    "data://patch/patch1_initial/patchindex.ebex@");
                SQEX::Luminous::AssetManager::LmFileList::AddPatchIndexEarc(
                    &patchIndexId);

                // This call is needed to finish this off, but it's not 100%
                // clear what it does
                const auto assetManager =
                    SQEX::Luminous::AssetManager::LmGetAssetManager();
                const auto assetManagerBaseAddress =
                    *static_cast<uint64_t*>(assetManager);
                const auto acquireAsset = *reinterpret_cast<AcquireAsset_t*>(
                    assetManagerBaseAddress + 8);
                acquireAsset(assetManager, &patchIndexId, 0, 2);
            }
            CPPTRACE_CATCH(...)
            {
                Exception::Fatal(
                    "Failed to add patch1_initial to asset manager");
            }

            SPDLOG_INFO("Finished adding patch1_initial to asset manager");
        });

        return original_(pApplicationBase);
    }

  public:
    bool ShouldApply() override
    {
        return true;
    }
};
} // namespace Hooks

#endif // PATCH1INITIALHOOK_H
