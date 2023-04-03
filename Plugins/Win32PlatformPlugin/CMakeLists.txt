cmake_minimum_required(VERSION 3.8)

project(Win32PlatformPlugin)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_plugin_target_out_path(Win32PlatformPlugin SHARED "VT_Plugins" ${VT_PLUGIN_OUT})

target_link_libraries(Win32PlatformPlugin
	PUBLIC
		Core
		Platform
		PluginSystem_COMMON
		WindowSystem_COMMON
		InputSystem
)