#ifndef PATCHMANAGER_H
#define PATCHMANAGER_H
#include <vector>

#include "IPatch.h"
#include "MemorySignature.h"

namespace Patches
{
    /**
     * Handles applying patches to the game and cleaning up when the application closes.
     */
    class PatchManager
    {
    private:
        std::vector<IPatch*> patches_;

        PatchManager() = default;

        ~PatchManager()
        {
            for (const auto patch: patches_)
            {
                delete patch;
            }
        }

    public:
        static PatchManager& GetInstance()
        {
            static PatchManager instance;
            return instance;
        }

        PatchManager(const PatchManager&) = delete;

        PatchManager& operator=(const PatchManager&) = delete;

        /**
         * Registers a patch with the patch manager.
         * @tparam TPatch Class type of the patch to register.
         */
        template<typename TPatch>
        void Register()
        {
            patches_.push_back(new TPatch());
        }

        /**
         * Applies all registered patches to the game.
         * @remarks Patches will only be applied if their ShouldApply function returns true.
         */
        void ApplyPatches() const
        {
            for (const auto patch: patches_)
            {
                if (patch->ShouldApply())
                {
                    // Validate expected target count
                    auto expected = patch->GetExpectedTargetCount();
                    if (expected == 0)
                    {
                        throw cpptrace::invalid_argument("IPatch.GetExpectedTargetCount must not equal 0");
                    }

                    // Apply the patch
                    std::string name(typeid(*patch).name());
                    SPDLOG_INFO("Applying patch: {}", name);
                    auto target = MemorySignature(patch->GetTargetSignature());
                    const auto replacement = MemorySignature(patch->GetPatchSignature());
                    const auto actual = target.Replace(replacement);

                    // Ensure the patch was applied as expected
                    if (expected < 1 && actual == 0)
                    {
                        throw cpptrace::runtime_error("Failed to apply patch: " + name);
                    }

                    if (actual != expected)
                    {
                        SPDLOG_CRITICAL("Failed to apply patch: {}", name);
                        SPDLOG_CRITICAL("Expected {} occurrences of target signature, found {}", expected, actual);
                        throw cpptrace::runtime_error("Failed to apply patch: " + name);
                    }

                    SPDLOG_INFO("Applied patch: {}", name);
                }
            }
        }
    };
}

#endif //PATCHMANAGER_H
