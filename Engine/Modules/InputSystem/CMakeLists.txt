cmake_minimum_required(VERSION 3.8)

project(InputSystem)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(InputSystem INTERFACE "VT_Engine/UserInterfaces")

target_link_libraries(InputSystem
	INTERFACE
		Math
)