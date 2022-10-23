cmake_minimum_required(VERSION 3.8)

project(Platform)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(Platform INTERFACE "VT_Engine")

target_link_libraries(Platform
    INTERFACE
        Core
)