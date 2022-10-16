cmake_minimum_required(VERSION 3.8)

project(FileSystem)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(FileSystem STATIC "VT_Engine/FileSystem")
vt_add_common_module(FileSystem)

target_link_libraries(FileSystem
	PUBLIC
		FileSystem_COMMON
)