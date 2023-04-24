cmake_minimum_required(VERSION 3.8)

project(GraphRender)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(GraphRender STATIC "VT_Engine/Graphics/Renderer/RenderSystem")

target_link_libraries(GraphRender
	PUBLIC
		Render
)