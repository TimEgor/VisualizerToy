cmake_minimum_required(VERSION 3.8)

project(LevelSystem)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(LevelSystem STATIC "VT_Engine/LevelSystem")
vt_add_common_module(LevelSystem)

target_link_libraries(LevelSystem
	PUBLIC
		Scene
)