cmake_minimum_required(VERSION 3.8)

project(DebugUiSystemCommon)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_common_module_target(DebugUiSystem INTERFACE "VT_Engine/Debugging/Ui")

target_link_libraries(DebugUiSystem_COMMON
	INTERFACE
		ImGui_COMMON
)