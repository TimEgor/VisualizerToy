cmake_minimum_required(VERSION 3.8)

project(MeshSystem)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(MeshSystem STATIC "VT_Engine/Graphics/Renderer/MeshSystem")
vt_add_common_module(MeshSystem)

target_link_libraries(MeshSystem
	PUBLIC
		MeshSystem_COMMON
		Core
		Engine
		NamedReferencePool
)