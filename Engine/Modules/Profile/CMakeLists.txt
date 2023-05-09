cmake_minimum_required(VERSION 3.8)

project(Profile)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)
include(${VT_ROOT_PATH}/CMake/PreBuild.cmake)

vt_add_module_target(Profile STATIC "VT_Engine")

target_link_libraries(Profile
    PUBLIC
        Core
)

vt_check_and_build_module(WinPixEventRuntime FALSE)
if ((${WinPixEventRuntime_FOUND}) AND (DEFINED WinPixEventRuntime_FOUND))
    target_compile_definitions(Profile PUBLIC VT_PROFILE_PIX)
    vt_add_third_party_module(Profile PUBLIC WinPixEventRuntime WinPixEventRuntime)
endif()