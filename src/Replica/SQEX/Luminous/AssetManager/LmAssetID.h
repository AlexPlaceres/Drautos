#ifndef LMASSETID_H
#define LMASSETID_H
#include <cstdint>
#include <string>

#include "../Core.h"

namespace SQEX::Luminous::AssetManager
{
/**
 * Represents an identifier for a game asset.
 */
struct LmAssetID
{
  private:
    explicit LmAssetID(const char* uri)
    {
        fullHash_ = 0;
        m_path = uri;
    }

  public:
    /**
     * The raw value of the asset ID. This is a combined hash of the URI and
     * type (file extension).
     */
    uint64_t fullHash_;

    /**
     * The URI associated with the asset.
     */
    std::string m_path;

    /**
     * Gets the URI hash for an asset.
     * @param pAssetId The ID of the asset to get the URI hash for.
     * @param result The computed URI hash, same as the return value.
     * @return The computed URI hash.
     * @remarks If the URI hash has already been computed, the stored value is
     * returned, otherwise it will be computed, stored,and then returned.
     */
    static uint64_t GetNameHash(LmAssetID* pAssetId, uint64_t* result)
    {
        const auto uri_hash = pAssetId->fullHash_ & 0x00000FFFFFFFFFFF;
        const auto type_hash = pAssetId->fullHash_ & 0xFFFFF00000000000;
        *result = uri_hash;

        if (uri_hash == 0)
        {
            uint64_t name_hash_result;
            const auto new_uri_hash =
                GenerateNameHash(&name_hash_result, &pAssetId->m_path);
            *result = new_uri_hash;
            pAssetId->fullHash_ = type_hash | new_uri_hash;
        }

        return *result;
    }

    /**
     * Generates the hash of the given URI, excluding the file extension (type)
     * component.
     * @param result The location to store the resulting hash, same as the
     * return value.
     * @param path The URI to hash.
     * @return The computed hash.
     */
    static uint64_t GenerateNameHash(uint64_t* result, const std::string* path)
    {
        *result = Core::Fnv1a64Lower(path->data(), 0x14650FB0739D0383) &
                  0xFFFFFFFFFFF;
        return *result;
    }

    /**
     * Creates a new unhashed AssetID with the URI populated.
     * @param uri The URI to initialize the asset ID with.
     * @return The newly created LmAssetID.
     */
    static LmAssetID Create(const char* uri)
    {
        return LmAssetID(uri);
    }
};
} // namespace SQEX::Luminous::AssetManager

#endif // LMASSETID_H
