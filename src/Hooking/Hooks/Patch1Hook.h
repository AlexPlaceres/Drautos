#ifndef PATCH1HOOK_H
#define PATCH1HOOK_H

#include <cpptrace/from_current.hpp>
#include <mutex>

#include "../FunctionHook.h"

#include "../../Logging/Exception.h"
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
 * Black::Menu::FontUtil* __fastcall
 * SQEX::Luminous::Core::Singleton<Black::Menu::FontUtil>::Get()
 * @endcode
 * @remarks This is one of two hooks that adds a patch archive to the asset
 * manager. This one is hooked later in the game's initialization process, as
 * calling it earlier was causing visual anomalies once the game was loaded,
 * which seemed possibly related to shader passes.
 */
class Patch1Hook final : public FunctionHook<0x3a3e70, 0x36cb50, void*>
{
private:
    using AcquireAsset_t = void*(__fastcall*)(void* pAssetManager, LmAssetID*,
                                              uint64_t, int64_t);

    std::once_flag hasApplied_;

protected:
    /**
     * Gets the singleton instance of the font utilities class.
     * @return The font utilities instance.
     * @remarks This detour will add patch/patch1/patchindex.earc to the asset
     * manager the first time it is called.
     */
    void* Detour() override
    {
        std::call_once(hasApplied_, [] {
            SPDLOG_INFO("Adding patch1 to asset manager");

            CPPTRACE_TRY
            {
                // Add patch1 to the asset manager
                auto patchIndexId =
                    LmAssetID::Create("data://patch/patch1/patchindex.ebex@");
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
                Exception::Fatal("Failed to add patch1 to asset manager");
            }

            SPDLOG_INFO("Finished adding patch1 to asset manager");
        });

        return original_();
    }

public:
    bool ShouldApply() override
    {
        return true;
    }
};
} // namespace Hooks

#endif // PATCH1HOOK_H
