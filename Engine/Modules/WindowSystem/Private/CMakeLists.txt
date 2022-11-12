cmake_minimum_required(VERSION 3.8)

project(WindowSystem)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(WindowSystem STATIC "VT_Engine/UserInterfaces/WindowSystem")
vt_add_common_module(WindowSystem)

target_link_libraries(WindowSystem
	PUBLIC
		Core
		Engine_COMMON
		WindowSystem_COMMON
		Platform
)