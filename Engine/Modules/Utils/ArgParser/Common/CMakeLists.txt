cmake_minimum_required(VERSION 3.8)

project(ArgParserCommon)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_common_module_target(ArgParser INTERFACE "VT_Engine/Utils/ArgParser")