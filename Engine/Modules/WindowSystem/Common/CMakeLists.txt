cmake_minimum_required(VERSION 3.8)

project(WindowSystemCommon)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_common_module_target(WindowSystem STATIC "VT_Engine/UserInterfaces/WindowSystem")

target_link_libraries(WindowSystem_COMMON
	PUBLIC
		Math
		Engine_COMMON
		EventSystem_COMMON
)