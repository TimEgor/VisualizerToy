cmake_minimum_required(VERSION 3.8)

project(Scene)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(Scene STATIC "VT_Engine/Scene")
vt_add_common_module(Scene)
vt_add_ecs_module(Scene)

target_link_libraries(Scene
	PUBLIC
		ObjectPool
		Containers
)