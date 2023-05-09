cmake_minimum_required(VERSION 3.8)

project(GameSystem)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(GameSystem STATIC "VT_Engine/GameSystem")
vt_add_common_module(GameSystem)

target_link_libraries(GameSystem
	PRIVATE
		Engine_COMMON
		Profile
)