#ifndef UNMASKCOMPRESSEDHOOK_H
#define UNMASKCOMPRESSEDHOOK_H
#include "../FunctionHook.h"

#include "../../Host.h"

namespace Hooks
{
    /**
     * Unmasks the compressed flag when reading archives.\n\n
     * Hooks the following method:
     * @code
     * SQEX::Luminous::AssetManager::LmArcInterface::ArcAssociation* SQEX::Luminous::AssetManager::LmArcInterface::FindAssetInfo(
     *     SQEX::Luminous::AssetManager::LmArcInterface* this,
     *     const SQEX::Luminous::AssetManager::LmAssetID* id)
     * @endcode
     * @remarks The compressed flag is replaced with a different flag when Flagrum builds mods.
     *          This is to prevent users from being able to rip mod assets from other peoples' work by
     *          trying to extract them with other tools. This hook undoes this masking so the game knows
     *          how to read them again.
     */
    class UnmaskCompressedHook final : public FunctionHook<0xD0C7D0, 0xC1C520, void*, void*, void*>
    {
    private:
        const int16_t COMPRESSED = 2;
        const int16_t MASK_COMPRESSED = 256;

    protected:
        bool ShouldApply() override { return true; }

        /**
         * Retrieves information about an asset from an archive.
         * @param pArchiveInterface The archive that is being searched for asset information.
         * @param pAssetId The ID of the asset that is being searched for.
         * @return The requested asset information.
         * @remarks This detour removes the fake MASK_COMPRESSED flag from any archive entries that have it,
         *          replacing it with the real COMPRESSED flag.
         */
        void* Detour(void* pArchiveInterface, void* pAssetId) override
        {
            const auto asset = original_(pArchiveInterface, pAssetId);

            if (asset)
            {
                const auto assetEntry = reinterpret_cast<void*>(
                    *reinterpret_cast<int64_t*>(static_cast<char*>(asset) + 8));
                const auto offset = DISCERN(0x58, 0x38);
                const auto pFlags = reinterpret_cast<int16_t*>(static_cast<char*>(assetEntry) + offset);

                if ((*pFlags & MASK_COMPRESSED) != 0)
                {
                    *pFlags &= ~MASK_COMPRESSED; // NOLINT(*-narrowing-conversions)
                    *pFlags |= COMPRESSED; // NOLINT(*-narrowing-conversions)
                }
            }

            return asset;
        }
    };
}

#endif //UNMASKCOMPRESSEDHOOK_H

