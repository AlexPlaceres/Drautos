#ifndef CORE_H
#define CORE_H
#include <cstdint>

namespace SQEX::Luminous::Core
{
    /**
     * Generates an FNV1A hash of the given string, converting all uppercase letters to lowercase before hashing.
     * @param pString The string to hash.
     * @param offsetBasis The FNV1A offset basis to use for the hash calculation.
     * @return The computed hash.
     */
    inline uint64_t Fnv1a64Lower(const char* pString, uint64_t offsetBasis)
    {
        auto current = *pString;

        // Iterate through each character in the string
        for (auto i = pString; *i; current = *i)
        {
            // Convert uppercase letters to lowercase
            if (static_cast<uint8_t>(current - 65) <= 0x19)
            {
                current += 32;
            }

            // Update the hash
            i++;
            offsetBasis = 0x100000001B3 * (current ^ offsetBasis);
        }

        // Return the final hash
        return offsetBasis;
    }
}

#endif //CORE_H
