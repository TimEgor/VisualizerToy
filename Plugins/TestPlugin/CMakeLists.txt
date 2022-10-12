cmake_minimum_required(VERSION 3.8)

project(TestPlugin)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_plugin_target_out_path(TestPlugin SHARED "VT_Plugins" ${VT_PLUGIN_OUT})

target_link_libraries(TestPlugin
	PUBLIC
		Core
		PluginSystem_COMMON
)