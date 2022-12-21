cmake_minimum_required(VERSION 3.8)

project(MaterialSystemCommon)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_common_module_target(MaterialSystem INTERFACE "VT_Engine/Graphics/Renderer/MaterialSystem")

target_link_libraries(MaterialSystem_COMMON
	INTERFACE
		Shaders
		GraphicResourceManager_COMMON
)