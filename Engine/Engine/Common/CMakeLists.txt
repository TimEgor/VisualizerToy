cmake_minimum_required(VERSION 3.8)

project(EngineCommon)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_common_module_target(Engine INTERFACE "VT_Engine/Engine/")

target_link_libraries(Engine_COMMON
	INTERFACE
		Core
)