cmake_minimum_required(VERSION 3.8)

project(LevelSystemCommon)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_common_module_target(LevelSystem INTERFACE "VT_Engine/LevelSystem")

target_link_libraries(LevelSystem_COMMON
	INTERFACE
        Core
		EntityComponentSystem
)