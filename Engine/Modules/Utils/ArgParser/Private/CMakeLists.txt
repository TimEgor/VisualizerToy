cmake_minimum_required(VERSION 3.8)

project(ArgParser)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(ArgParser STATIC "VT_Engine/Utils/ArgParser")
vt_add_common_module(ArgParser)

target_link_libraries(ArgParser
	PUBLIC
		Core
)