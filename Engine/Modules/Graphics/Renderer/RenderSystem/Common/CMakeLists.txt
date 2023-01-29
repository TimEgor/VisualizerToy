cmake_minimum_required(VERSION 3.8)

project(RenderSystemCommon)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_common_module_target(RenderSystem INTERFACE "VT_Engine/Graphics/Renderer/RenderSystem")

target_link_libraries(RenderSystem_COMMON
	INTERFACE
		GraphicResourceManager_COMMON
		GraphicCommandResources
)