cmake_minimum_required(VERSION 3.8)

project(SceneCommon)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_common_module_target(Scene INTERFACE "VT_Engine/Scene")

target_link_libraries(Scene_COMMON
	INTERFACE
        Core
		Math
)