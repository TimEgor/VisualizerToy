cmake_minimum_required(VERSION 3.8)

project(CameraECS)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_ecs_module_target(Camera INTERFACE "VT_Engine/Graphics/Renderer/Camera")

target_link_libraries(Camera_ECS
	INTERFACE
		Camera
)