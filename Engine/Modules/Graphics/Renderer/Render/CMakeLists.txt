cmake_minimum_required(VERSION 3.8)

project(Render)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(Render INTERFACE "VT_Engine/Graphics/Renderer/RenderSystem")

target_link_libraries(Render
	INTERFACE
		GraphicResourceManager_COMMON
)