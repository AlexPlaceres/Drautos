#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H
#include "../../../Host.h"

namespace SQEX::Luminous::AssetManager
{
/**
 * Gets the singleton instance of the Luminous asset manager.
 * @return The asset manager instance.
 */
inline void* LmGetAssetManager()
{
    using GetAssetManager_t = void* (*)();
    const auto address = REBASE(0xD2BF90, 0xC2C750);
    const auto getAssetManager = reinterpret_cast<GetAssetManager_t>(address);
    return getAssetManager();
}
} // namespace SQEX::Luminous::AssetManager

#endif // ASSETMANAGER_H
