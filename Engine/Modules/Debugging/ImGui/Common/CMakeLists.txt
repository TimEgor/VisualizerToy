cmake_minimum_required(VERSION 3.8)

project(ImGuiCommon)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_common_module_target(ImGui INTERFACE "VT_Engine/Debugging/ImGui")

target_include_directories(ImGui_COMMON
	INTERFACE
		"${VT_ROOT_PATH}/Libs/imgui/"
)