cmake_minimum_required(VERSION 3.8)

project(GraphicResourceManagerCommon)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_common_module_target(GraphicResourceManager INTERFACE "VT_Engine/Graphics/Device/ResourceManager")

target_link_libraries(GraphicResourceManager_COMMON
	INTERFACE
		ObjectPool
)