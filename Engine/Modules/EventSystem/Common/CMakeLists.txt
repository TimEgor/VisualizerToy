cmake_minimum_required(VERSION 3.8)

project(EventSystemCommon)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_common_module_target(EventSystem INTERFACE "VT_Engine/EventSystem")

target_link_libraries(EventSystem_COMMON
	INTERFACE
		Core
)