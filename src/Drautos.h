﻿#ifndef DRAUTOS_H
#define DRAUTOS_H

#include "Hooking/FunctionHookManager.h"
#include "Hooking/Hooks/Patch1Hook.h"
#include "Hooking/Hooks/Patch1InitialHook.h"
#include "Hooking/Hooks/SnapshotLimitHook.h"
#include "Hooking/Hooks/SteamRestartHook.h"
#include "Hooking/Hooks/UnlockDlcHook.h"
#include "Hooking/Hooks/UnmaskCompressedHook.h"
#include "Host.h"
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
        auto& patchManager = Patches::PatchManager::GetInstance();
        patchManager.Register<Patches::AnselPatch>();
        patchManager.Register<Patches::TwitchPrimePatch>();
        patchManager.ApplyPatches();
    }

    static void ApplyHooks()
    {
        auto& hookManager = Hooks::FunctionHookManager::GetInstance();
        hookManager.Register<Hooks::UnmaskCompressedHook>();
        hookManager.Register<Hooks::Patch1InitialHook>();
        hookManager.Register<Hooks::Patch1Hook>();
        hookManager.Register<Hooks::SnapshotLimitHook>();
        hookManager.Register<Hooks::UnlockDlcHook>();
        hookManager.Register<Hooks::SteamRestartHook>();
        hookManager.ApplyHooks();
    }
};

#endif // DRAUTOS_H
