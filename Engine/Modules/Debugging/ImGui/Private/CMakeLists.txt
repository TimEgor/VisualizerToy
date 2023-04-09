cmake_minimum_required(VERSION 3.8)

project(ImGui)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(ImGui STATIC "VT_Engine/Debugging/ImGui")
vt_add_common_module(ImGui)

target_link_libraries(ImGui
	PUBLIC
		Engine_COMMON
		WindowSystem_COMMON
		Platform
		GraphicPlatform
)