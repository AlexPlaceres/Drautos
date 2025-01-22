#ifndef MEMORYSIGNATURE_H
#define MEMORYSIGNATURE_H
#include <algorithm>
#include <array>
#include <stdexcept>
#include <vector>

#include "../Host.h"

/**
 * Represents one byte in a memory signature.
 */
struct MemorySignatureByte
{
    /**
     * If true, overrides Value to match any byte
     */
    bool IsWildcard{false};

    /**
     * The value to match.
     */
    uint8_t Value{0};

    bool operator==(const uint8_t byte) const
    {
        return IsWildcard || Value == byte;
    }
};

/**
 * Represents a contiguous pattern of memory.
 */
struct MemorySignature
{
  private:
    /**
     * Parses a hex character.
     * @param value The character to parse.
     * @return The byte represented by the hex character.
     */
    static uint8_t ParseOctet(const char& value)
    {
        // Parse digits
        if (value >= '0' && value <= '9')
        {
            return value - '0';
        }

        // Parse uppercase letters
        if (value >= 'A' && value <= 'F')
        {
            return 10 + value - 'A';
        }

        // Parse lowercase letters
        if (value >= 'a' && value <= 'f')
        {
            return 10 + value - 'a';
        }

        throw std::invalid_argument("Invalid hex character.");
    }

  public:
    /**
     * Instantiates a new MemorySignature from a hexadecimal string.
     * @param hexString String representation of a hexadecimal memory pattern.
     */
    explicit MemorySignature(const char* hexString)
    {
        for (auto i = 0; i < strlen(hexString); i += 2)
        {
            // Skip whitespace characters
            if (hexString[i] == ' ')
            {
                i -= 1;
                continue;
            }

            if (hexString[i] == '?' && hexString[i + 1] == '?')
            {
                // Value is a wildcard
                Signature[Size++].IsWildcard = true;
            }
            else
            {
                // Value is a hex character
                Signature[Size++].Value = ParseOctet(hexString[i]) << 4 |
                                          ParseOctet(hexString[i + 1]);
            }
        }
    }

    /**
     * The byte pattern that makes up the signature.
     */
    std::array<MemorySignatureByte, 128> Signature;

    /**
     * The size of the signature, in bytes.
     */
    size_t Size{0};

    /**
     * Finds this signature within the host process module.
     * @return List of pointers to the matching signatures.
     */
    std::vector<uint8_t*> Find()
    {
        std::vector<uint8_t*> results;

        // Determine bounds of the module
        const auto moduleStart = reinterpret_cast<uint8_t*>(Host::hModule);
        const auto moduleEnd = moduleStart + Host::ModuleSize;
        auto current = moduleStart;

        // Iterate the entire module
        while (current < moduleEnd)
        {
            // Check if the memory is accessible
            MEMORY_BASIC_INFORMATION memoryInfo;
            if (!VirtualQuery(current, &memoryInfo, sizeof(memoryInfo)) ||
                memoryInfo.State != MEM_COMMIT ||
                memoryInfo.Protect & PAGE_GUARD)
            {
                continue;
            }

            // Determine bounds of this memory region
            const auto start = static_cast<uint8_t*>(memoryInfo.BaseAddress);
            const auto end = start + memoryInfo.RegionSize;

            // Search for the signature in the region
            auto found = std::search(
                start, end, Signature.begin(),
                Signature.begin() + Size); // NOLINT(*-narrowing-conversions)

            while (found < end && found >= start)
            {
                results.push_back(found);
                found =
                    std::search(found + Size, end, Signature.begin(),
                                Signature.begin() +
                                    Size); // NOLINT(*-narrowing-conversions)
            }

            // Reset for the next region
            current = end;
            memoryInfo = {};
        }

        return results;
    }

    /**
     * Replaces all occurrences of this byte pattern in the game's memory with
     * the given patch.
     * @param patch The byte pattern to replace this signature with.
     * @return The number of matches of this signature that were replaced.
     */
    int Replace(const MemorySignature& patch)
    {
        const auto matches = Find();

        if (matches.size() == 0)
        {
            throw cpptrace::runtime_error("Failed to find signature.");
        }

        for (const auto current : matches)
        {
            // Make the memory writable
            DWORD oldProtection;
            VirtualProtect(current, 128, PAGE_EXECUTE_READWRITE,
                           &oldProtection);

            // Patch the signature
            for (auto p = 0; p < patch.Size; p++)
            {
                auto [IsWildcard, Value] = patch.Signature[p];
                if (!IsWildcard)
                {
                    current[p] = Value;
                }
            }

            // Restore the memory protection
            VirtualProtect(current, 128, oldProtection, &oldProtection);
        }

        return matches.size();
    }
};

#endif // MEMORYSIGNATURE_H
