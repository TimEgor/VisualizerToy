cmake_minimum_required(VERSION 3.8)

project(MaterialSystem)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(MaterialSystem STATIC "VT_Engine/Graphics/Renderer/MaterialSystem")
vt_add_common_module(MaterialSystem)

target_link_libraries(MaterialSystem
	PUBLIC
		MaterialSystem_COMMON

		Core
		Engine_COMMON
		NamedReferencePool
)