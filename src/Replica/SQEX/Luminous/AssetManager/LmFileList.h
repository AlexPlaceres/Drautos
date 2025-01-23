#ifndef LMFILELIST_H
#define LMFILELIST_H

#include <vector>

#include "LmAssetID.h"

#include "../../../../Host.h"

using LmAssetId = SQEX::Luminous::AssetManager::LmAssetID;

namespace SQEX::Luminous::AssetManager
{
/**
 * Represents a list of files handled by the Luminous asset manager.
 */
class LmFileList
{
private:
    inline static bool hasInitializedVector_;

public:
    /**
     * Adds a patch index archive to the asset manager's file list.
     * @param lmAssetID The identifier of the patch index asset.
     * @remarks This is a recreation of the function from debug as it doesn't
     * appear to exist in release.
     */
    static void AddPatchIndexEarc(LmAssetID* lmAssetID)
    {
        // Grab the patch index list
        const auto lmFileList =
            reinterpret_cast<char*>(REBASE(0x5ADFB90, 0x4CE7C18));
        const auto pPatchIndexEarcList =
            reinterpret_cast<std::vector<uint64_t>*>(lmFileList + 0x138);

        // For some accursed reason, the vector crashes on push_back on the
        // debug build if we don't create a new one
        if (!hasInitializedVector_ && Host::Type == Configuration::DEBUG)
        {
            hasInitializedVector_ = true;
            *pPatchIndexEarcList = std::vector<uint64_t>();
        }

        // Add the URI hash to the patch index list
        uint64_t result;
        const auto nameHash = LmAssetID::GetNameHash(lmAssetID, &result);
        pPatchIndexEarcList->push_back(nameHash);
    }
};
} // namespace SQEX::Luminous::AssetManager

#endif // LMFILELIST_H
