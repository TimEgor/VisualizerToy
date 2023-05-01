cmake_minimum_required(VERSION 3.8)

project(FreeCameraController)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(FreeCameraController STATIC "VT_Demo_Common")

target_link_libraries(FreeCameraController
	PUBLIC
		Core

		Engine_COMMON
		LevelSystem_COMMON
		EntityComponentSystem
		InputSystem

		Scene_ECS
		Camera_ECS
)