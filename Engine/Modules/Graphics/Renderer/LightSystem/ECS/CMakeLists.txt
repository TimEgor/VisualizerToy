cmake_minimum_required(VERSION 3.8)

project(LightSystemECS)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_ecs_module_target(LightSystem INTERFACE "VT_Engine/Graphics/Renderer/LightSystem")

target_link_libraries(LightSystem_ECS
	INTERFACE
		Math
)