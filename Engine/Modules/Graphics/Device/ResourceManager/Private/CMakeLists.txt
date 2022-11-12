cmake_minimum_required(VERSION 3.8)

project(GraphicResourceManager)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(GraphicResourceManager STATIC "VT_Engine/Graphics/Device/ResourceManager")
vt_add_common_module(GraphicResourceManager)

target_link_libraries(GraphicResourceManager
	PUBLIC
		Core
		ObjectPool
		Engine_COMMON
		GraphicDevice
		GraphicResourceManager_COMMON
)