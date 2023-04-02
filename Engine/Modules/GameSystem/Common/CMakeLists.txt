cmake_minimum_required(VERSION 3.8)

project(GameSystemCommon)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_common_module_target(GameSystem INTERFACE "VT_Engine/GameSystem")

target_link_libraries(GameSystem_COMMON
	INTERFACE
		PluginSystem_COMMON
)