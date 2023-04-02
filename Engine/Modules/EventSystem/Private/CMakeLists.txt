cmake_minimum_required(VERSION 3.8)

project(EventSystem)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(EventSystem STATIC "VT_Engine/EventSystem")
vt_add_common_module(EventSystem)

target_link_libraries(EventSystem
	PUBLIC
		ObjectPool
)