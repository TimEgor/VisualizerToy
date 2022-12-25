cmake_minimum_required(VERSION 3.8)

project(GraphicSynchronization)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(GraphicSynchronization INTERFACE "VT_Engine/Graphics/Device")