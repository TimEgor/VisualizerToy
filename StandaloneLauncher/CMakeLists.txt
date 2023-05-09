cmake_minimum_required(VERSION 3.8)

project(VT_StandaloneLauncher)

set(VT_ROOT_PATH ${CMAKE_CURRENT_LIST_DIR}/../)
set(VT_PLUGIN_DIR ${VT_ROOT_PATH}/Plugins)
set(VT_PLUGIN_BINARY ${VT_PLUGIN_DIR}/Build)
set(VT_PLUGIN_OUT ${VT_PLUGIN_DIR}/Out)
set(VT_ASSET_CONVERTERS_DIR ${VT_ROOT_PATH}/AssetConverters)
set(VT_ASSET_CONVERTERS_BINARY ${VT_ASSET_CONVERTERS_DIR}/Build)
set(VT_ASSET_CONVERTERS_OUT ${VT_ASSET_CONVERTERS_DIR}/Out)

include(${VT_ROOT_PATH}/CMake/Core.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)
include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)

set(ALL_SRCS)
vt_get_all_sources_recurse("${CMAKE_CURRENT_LIST_DIR}/Sources" "StandaloneLauncher" ALL_SRCS)
vt_generate_source_groups_by_sources("${CMAKE_CURRENT_LIST_DIR}/Sources" "${ALL_SRCS}")

#add_compile_options(-fsanitize=address)

add_executable(StandaloneLauncher WIN32 ${ALL_SRCS})

vt_set_module_default_target_properties_out_path(StandaloneLauncher ${CMAKE_CURRENT_LIST_DIR}/Out/${CMAKE_GENERATOR_PLATFORM})
set_target_properties(StandaloneLauncher PROPERTIES
	FOLDER "VT_StandaloneLauncher"
)

get_target_property(OUTPUT_LOCATION StandaloneLauncher RUNTIME_OUTPUT_DIRECTORY)

set(MODULES_SET
	Engine		Engine/Private
)

vt_add_modules_by_path(StandaloneLauncher PUBLIC
	"${MODULES_SET}" ${CMAKE_CURRENT_LIST_DIR}/../Engine/ ${CMAKE_CURRENT_BINARY_DIR}/Core)

message("\n>>>>>> PLUGINS <<<<<<<")
vt_add_plugins_include(Engine
	"EnginePlugins.cmake" ${VT_PLUGIN_DIR} ${VT_PLUGIN_BINARY})

message("\n>>>>>> CONVERTERS <<<<<<<")
vt_add_plugins_include(Engine
	"EngineConverters.cmake" ${VT_ASSET_CONVERTERS_DIR} ${VT_ASSET_CONVERTERS_BINARY})

message("\n----------------------")

if (TARGET WinPixEventRuntime)
	add_custom_command(TARGET StandaloneLauncher POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${VT_ROOT_PATH}/Libs/WinPixEventRuntime/bin/x64/WinPixEventRuntime.dll ${OUTPUT_LOCATION}
	)
endif()

if (MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT StandaloneLauncher)
	set_target_properties(StandaloneLauncher PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY ${OUTPUT_LOCATION}
	)
endif()