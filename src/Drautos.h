#ifndef DRAUTOS_H
#define DRAUTOS_H
#include <spdlog/spdlog.h>

#include "Host.h"
#include "Hooking/FunctionHookManager.h"
#include "Hooking/Hooks/Patch1Hook.h"
#include "Hooking/Hooks/Patch1InitialHook.h"
#include "Hooking/Hooks/SnapshotLimitHook.h"
#include "Hooking/Hooks/UnlockDlcHook.h"
#include "Hooking/Hooks/UnmaskCompressedHook.h"
#include "Patching/PatchManager.h"
#include "Patching/Patches/AnselPatch.h"
#include "Patching/Patches/TwitchPrimePatch.h"

class Drautos
{
public:
    static void Run()
    {
        Host::Initialize();
        ApplyPatches();
        ApplyHooks();
    }

private:
    static void ApplyPatches()
    {
        SPDLOG_INFO("Registering patches");
        auto& patchManager = Patches::PatchManager::GetInstance();
        patchManager.Register<Patches::AnselPatch>();
        patchManager.Register<Patches::TwitchPrimePatch>();

        SPDLOG_INFO("Applying patches");
        patchManager.ApplyPatches();
        SPDLOG_INFO("Patches applied");
    }

    static void ApplyHooks()
    {
        SPDLOG_INFO("Registering hooks");
        auto& hookManager = Hooks::FunctionHookManager::GetInstance();
        hookManager.Register<Hooks::UnmaskCompressedHook>();
        hookManager.Register<Hooks::Patch1InitialHook>();
        hookManager.Register<Hooks::Patch1Hook>();
        hookManager.Register<Hooks::SnapshotLimitHook>();
        hookManager.Register<Hooks::UnlockDlcHook>();

        SPDLOG_INFO("Applying hooks");
        hookManager.ApplyHooks();
        SPDLOG_INFO("Hooks applied");
    }
};

#endif //DRAUTOS_H
