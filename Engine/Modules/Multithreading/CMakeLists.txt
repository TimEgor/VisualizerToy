cmake_minimum_required(VERSION 3.8)

project(Multithreading)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(Multithreading INTERFACE "VT_Engine")

target_link_libraries(Multithreading
	INTERFACE
		Core
)