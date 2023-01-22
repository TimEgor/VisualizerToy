cmake_minimum_required(VERSION 3.8)

project(MeshSystemECS)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_ecs_module_target(MeshSystem INTERFACE "VT_Engine/Graphics/Renderer/MeshSystem")

target_link_libraries(MeshSystem_ECS
	INTERFACE
		MeshSystem_COMMON
)