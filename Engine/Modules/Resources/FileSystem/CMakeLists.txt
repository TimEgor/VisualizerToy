cmake_minimum_required(VERSION 3.8)

project(FileSystem)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(FileSystem INTERFACE "VT_Engine/Resources")

target_link_libraries(FileSystem
	INTERFACE
		Core
)