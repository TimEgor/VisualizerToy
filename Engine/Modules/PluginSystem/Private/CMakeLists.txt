cmake_minimum_required(VERSION 3.8)

project(PluginSystem)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(PluginSystem STATIC "VT_Engine/PluginSystem")
vt_add_common_module(PluginSystem)