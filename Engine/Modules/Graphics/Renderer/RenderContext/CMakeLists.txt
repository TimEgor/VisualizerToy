cmake_minimum_required(VERSION 3.8)

project(RenderContext)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(RenderContext STATIC "VT_Engine/Graphics/Renderer/RenderSystem")

target_link_libraries(RenderContext
	PUBLIC
		InputLayout
		GraphicResourceManager_COMMON
		MeshSystem_COMMON
)