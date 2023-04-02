cmake_minimum_required(VERSION 3.8)

project(ImGui)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(ImGui STATIC "VT_Engine/Debugging")

target_include_directories(ImGui
	PRIVATE
		${VT_ROOT_PATH}/Libs/imgui/
)