cmake_minimum_required(VERSION 3.8)

project(DefaultSceneRender)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(DefaultSceneRender STATIC "VT_Engine/Graphics/Renderer/RenderSystem")

target_link_libraries(DefaultSceneRender
	PUBLIC
		DefaultRender
		
		LevelSystem

		MeshSystem_ECS
		Scene_ECS
		LightSystem_ECS
		Camera_ECS
)