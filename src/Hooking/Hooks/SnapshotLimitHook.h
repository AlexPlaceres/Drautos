#ifndef SNAPSHOTLIMITHOOK_H
#define SNAPSHOTLIMITHOOK_H
#include "../FunctionHook.h"
#include "../../Configuration.h"

namespace Hooks
{
    /**
     * Increases the maximum number of Prompto snapshots that the game will allow the user to save.\n\n
     * Hooks the following method:
     * @code
     * __int64 __fastcall Black::AI::Buddy::Snapshot::BuddySnapshotStorage::GetSnapshotMaxNum(
     *     Black::AI::Buddy::Snapshot::BuddySnapshotStorage *this)
     * @endcode
     */
    class SnapshotLimitHook final : public FunctionHook<0x11CF2D0, 0x8FCBDE0, int64_t>
    {
    protected:
        /**
         * Returns the maximum number of snapshots the user is allowed to store.
         * @return The new snapshot limit.
         */
        int64_t Detour() override { return 9999; }

    public:
        bool ShouldApply() override { return Configuration::GetInstance().IncreaseSnapshotLimit; }
    };
}

#endif //SNAPSHOTLIMITHOOK_H
