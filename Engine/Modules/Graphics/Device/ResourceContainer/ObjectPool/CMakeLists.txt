cmake_minimum_required(VERSION 3.8)

project(GraphicResourceContainerObjectPool)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(GraphicResourceContainerObjectPool INTERFACE "VT_Engine/Graphics/Device/ResourceContainers")

target_link_libraries(GraphicResourceContainerObjectPool
	INTERFACE
		GraphicResourceManager_COMMON
		ObjectPool
)