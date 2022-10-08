cmake_minimum_required(VERSION 3.8)

project(VT_StandaloneLauncher)

set(VT_ROOT_PATH ${CMAKE_CURRENT_LIST_DIR}/../)

include(${VT_ROOT_PATH}/CMake/BuildingOptions.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)
include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)

set(ALL_SRCS)
vt_get_all_sources_recurse("${CMAKE_CURRENT_LIST_DIR}/Sources" "StandaloneLauncher" ALL_SRCS)
vt_generate_source_groups_by_sources("${CMAKE_CURRENT_LIST_DIR}/Sources" "${ALL_SRCS}")

add_executable(StandaloneLauncher WIN32 ${ALL_SRCS})

vt_set_module_default_target_properties_out_path(StandaloneLauncher ${CMAKE_CURRENT_LIST_DIR}/Out/${CMAKE_GENERATOR_PLATFORM})
set_target_properties(StandaloneLauncher PROPERTIES
	FOLDER "VT_StandaloneLauncher"
)

set(MODULES_SET
	Engine Engine/Engine/Private
)

get_target_property(OUTPUT_LOCATION StandaloneLauncher RUNTIME_OUTPUT_DIRECTORY)

vt_add_modules_by_path(StandaloneLauncher PUBLIC
	"${MODULES_SET}" ${CMAKE_CURRENT_LIST_DIR}/../ ${CMAKE_CURRENT_BINARY_DIR}/Core)

if (MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT StandaloneLauncher)
	set_target_properties(StandaloneLauncher PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY ${OUTPUT_LOCATION}
	)
endif()