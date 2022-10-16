cmake_minimum_required(VERSION 3.8)

project(Window)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(Window INTERFACE "VT_Engine/UserInterfaces")