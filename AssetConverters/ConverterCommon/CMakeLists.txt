cmake_minimum_required(VERSION 3.8)

project(ConverterCommon)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_plugin_target_out_path(ConverterCommon STATIC "VT_Converters" ${VT_ASSET_CONVERTERS_OUT})

target_link_libraries(ConverterCommon
	PUBLIC
		Core
		PluginSystem_COMMON
		ResourceConverter
		ResourceSystem
)