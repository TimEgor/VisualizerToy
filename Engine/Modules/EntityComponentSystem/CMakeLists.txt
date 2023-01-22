cmake_minimum_required(VERSION 3.8)

project(EntityComponentSystem)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(EntityComponentSystem STATIC "VT_Engine")

vt_add_third_party_module(EntityComponentSystem PUBLIC "EnTT" "EnTT::EnTT")

target_link_libraries(EntityComponentSystem
    PUBLIC
        Core
)