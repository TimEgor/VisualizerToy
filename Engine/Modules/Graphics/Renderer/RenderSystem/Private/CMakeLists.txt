cmake_minimum_required(VERSION 3.8)

project(RenderSystem)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(RenderSystem STATIC "VT_Engine/Graphics/Renderer/RenderSystem")
vt_add_common_module(RenderSystem)

target_link_libraries(RenderSystem
	PRIVATE
		Engine_COMMON
		EntityComponentSystem

		MeshSystem_ECS
		Scene_ECS
		LightSystem_ECS

		RenderContext

		GraphicDevice
		GraphicPipeline
		GraphicPlatform
		GraphicSynchronization
		Math

	PUBLIC
		Core
		MeshSystem_COMMON
		LevelSystem_COMMON
)