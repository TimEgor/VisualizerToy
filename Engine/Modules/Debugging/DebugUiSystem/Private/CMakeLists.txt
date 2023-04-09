cmake_minimum_required(VERSION 3.8)

project(DebugUiSystem)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(DebugUiSystem STATIC "VT_Engine/Debugging/Ui")
vt_add_common_module(DebugUiSystem)

target_link_libraries(DebugUiSystem
	PUBLIC
		ImGui
)