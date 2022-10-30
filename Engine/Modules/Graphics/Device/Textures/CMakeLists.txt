cmake_minimum_required(VERSION 3.8)

project(Textures)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(Textures INTERFACE "VT_Engine/Graphics/Device")