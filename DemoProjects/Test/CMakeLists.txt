cmake_minimum_required(VERSION 3.8)

project(VT_DemoTest)

set(VT_ROOT_PATH ${CMAKE_CURRENT_LIST_DIR}/../../)

include(${VT_ROOT_PATH}/CMake/BuildingOptions.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)
include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)

set(ALL_SRCS)
vt_get_all_sources_recurse("${CMAKE_CURRENT_LIST_DIR}/Sources" "DemoTest" ALL_SRCS)
vt_generate_source_groups_by_sources("${CMAKE_CURRENT_LIST_DIR}/Sources" "${ALL_SRCS}")

#add_compile_options(-fsanitize=address)

add_library(DemoTest SHARED ${ALL_SRCS})

vt_set_module_default_target_properties_out_path(DemoTest ${CMAKE_CURRENT_LIST_DIR}/Out/${CMAKE_GENERATOR_PLATFORM})
set_target_properties(DemoTest PROPERTIES
	FOLDER "VT_DemoTest")

set(VT_ENGINE_ECS TRUE)

set(ENGINE_MODULES_SET
	Engine_COMMON		Engine/Common)

vt_add_modules_by_path(DemoTest PUBLIC
	"${ENGINE_MODULES_SET}" ${VT_ROOT_PATH}/Engine/ ${CMAKE_CURRENT_BINARY_DIR}/Core)

message("\n----------------------")
message("\n>>>>>> DEMO MODULES <<<<<<<")

vt_add_modules_by_path_include(DemoTest PUBLIC
	"Modules.cmake" ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/Modules)

message("\n----------------------")

get_target_property(OUTPUT_LOCATION DemoTest RUNTIME_OUTPUT_DIRECTORY)

if (MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT DemoTest)
	set_target_properties(DemoTest PROPERTIES
		VS_DEBUGGER_COMMAND ${VT_ROOT_PATH}/StandaloneLauncher/Out/${CMAKE_GENERATOR_PLATFORM}/VT_StandaloneLauncher_$<CONFIG>_${CMAKE_GENERATOR_PLATFORM}.exe
		VS_DEBUGGER_COMMAND_ARGUMENTS "--module=${OUTPUT_LOCATION}/VT_DemoTest_$<CONFIG>_${CMAKE_GENERATOR_PLATFORM}.dll"

		VS_DEBUGGER_WORKING_DIRECTORY ${OUTPUT_LOCATION}
	)
endif()