cmake_minimum_required(VERSION 3.8)

project(RenderSystem)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(RenderSystem STATIC "VT_Engine/Graphics/Renderer/RenderSystem")
vt_add_common_module(RenderSystem)

target_link_libraries(RenderSystem
	PUBLIC
		Core
		Engine_COMMON
		RenderSystem_COMMON
		GraphicDevice
		GraphicPipeline
		GraphicPlatform
		Textures
		GPUBuffer
		InputLayout
		Math
)