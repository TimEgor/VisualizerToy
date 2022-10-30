cmake_minimum_required(VERSION 3.8)

project(WindowSystemCommon)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_common_module_target(WindowSystem INTERFACE "VT_Engine/UserInterfaces/WindowSystem")

target_link_libraries(WindowSystem_COMMON
    INTERFACE
        Math
        ObjectPool
)