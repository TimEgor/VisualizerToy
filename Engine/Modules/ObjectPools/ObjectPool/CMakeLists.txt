cmake_minimum_required(VERSION 3.8)

project(ObjectPool)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(ObjectPool INTERFACE "VT_Engine/ObjectPools")

target_link_libraries(ObjectPool
	INTERFACE
		Core
		Multithreading
)