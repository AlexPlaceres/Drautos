﻿cmake_minimum_required(VERSION 3.20)

# Setup vcpkg
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET x64-windows-static-md)

project(Drautos)

set(CMAKE_CXX_STANDARD 20)
add_definitions(-D_AMD64_)

# Find dependencies
find_path(DETOURS_INCLUDE_DIRS "detours/detours.h")
find_library(DETOURS_LIBRARY detours REQUIRED)

# Register project files
add_library(Drautos SHARED src/main.cpp
        src/Hooking/IFunctionHook.h
        src/Hooking/FunctionHookManager.h
        src/Hooking/FunctionHook.h
        src/Hooking/Hooks/UnmaskCompressedHook.h
        src/Host.h
        src/Hooking/Hooks/Patch1Hook.h
        src/Replica/SQEX/Luminous/AssetManager/LmAssetID.h
        src/Replica/SQEX/Luminous/Core.h
        src/Replica/SQEX/Luminous/AssetManager/LmFileList.h
        src/Replica/SQEX/Luminous/AssetManager.h
        src/Hooking/Hooks/SnapshotLimitHook.h
        src/Hooking/Hooks/Patch1InitialHook.h
        src/Configuration.h
        src/Hooking/Hooks/UnlockDlcHook.h
        src/Patching/IPatch.h
        src/Patching/MemorySignature.h
        src/Patching/Patches/AnselPatch.h
        src/Patching/PatchManager.h
        src/Patching/Patches/TwitchPrimePatch.h
        src/Logging/Exception.h
        src/Drautos.h
        src/Hooking/Hooks/SteamRestartHook.h
        src/Hooking/ExternFunctionHook.h
)

set_target_properties(Drautos PROPERTIES PREFIX "")

# Link dependencies
target_include_directories(Drautos PRIVATE ${DETOURS_INCLUDE_DIRS})
target_link_libraries(Drautos PRIVATE ${DETOURS_LIBRARY})