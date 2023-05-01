cmake_minimum_required(VERSION 3.8)

project(Camera)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(Camera INTERFACE "VT_Engine/Graphics/Renderer/Camera")
vt_add_ecs_module_link(Camera INTERFACE)

target_link_libraries(Camera
	INTERFACE
		Math
)