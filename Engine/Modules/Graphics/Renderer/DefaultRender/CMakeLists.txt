cmake_minimum_required(VERSION 3.8)

project(DefaultRender)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(DefaultRender STATIC "VT_Engine/Graphics/Renderer/RenderSystem")

target_link_libraries(DefaultRender
	PUBLIC
		Engine_COMMON

		GraphicDevice
		GraphRender

		RenderContext
)