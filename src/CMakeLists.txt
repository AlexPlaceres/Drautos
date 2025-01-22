cmake_minimum_required(VERSION 3.30)

# Setup vcpkg
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET x64-windows-static-md)

project(Drautos)

set(CMAKE_CXX_STANDARD 20)
add_definitions(-D_AMD64_)

# Find dependencies
find_package(cpptrace CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_path(DETOURS_INCLUDE_DIRS "detours/detours.h")
find_library(DETOURS_LIBRARY detours REQUIRED)

# Register project files
add_library(Drautos SHARED main.cpp
        Hooking/IFunctionHook.h
        Hooking/FunctionHookManager.h
        Hooking/FunctionHook.h
        Hooking/Hooks/UnmaskCompressedHook.h
        Host.h
        Hooking/Hooks/Patch1Hook.h
        Replica/SQEX/Luminous/AssetManager/LmAssetID.h
        Replica/SQEX/Luminous/Core.h
        Replica/SQEX/Luminous/AssetManager/LmFileList.h
        Replica/SQEX/Luminous/AssetManager.h
        Hooking/Hooks/SnapshotLimitHook.h
        Hooking/Hooks/Patch1InitialHook.h
        Configuration.h
        Hooking/Hooks/UnlockDlcHook.h
        Patching/IPatch.h
        Patching/MemorySignature.h
        Patching/Patches/AnselPatch.h
        Patching/PatchManager.h
        Patching/Patches/TwitchPrimePatch.h
        Logging/Logging.h
        Logging/Exception.h
        Drautos.h
)

set_target_properties(Drautos PROPERTIES PREFIX "")

# Link dependencies
target_link_libraries(Drautos PRIVATE cpptrace::cpptrace)
target_link_libraries(Drautos PRIVATE spdlog::spdlog)
target_include_directories(Drautos PRIVATE ${DETOURS_INCLUDE_DIRS})
target_link_libraries(Drautos PRIVATE ${DETOURS_LIBRARY})

add_custom_command(TARGET Drautos POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:Drautos> C:/Code/Flagrum-Dev/Flagrum.Desktop/bin/Release/net8.0-windows/fhook.dll)