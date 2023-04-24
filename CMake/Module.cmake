### vt_check_root_path
macro(vt_check_root_path)
	if (NOT DEFINED VT_ROOT_PATH)
		message(FATAL_ERROR "Error: VT_ROOT_PATH hasn't been defined.")
	endif()
endmacro()

### vt_set_module_default_target_properties
function(vt_set_module_default_target_properties TARGET_NAME)
	vt_set_module_default_target_properties_out_path(${TARGET_NAME} ${CMAKE_CURRENT_BINARY_DIR}/Out)
endfunction()

### vt_set_module_default_target_properties_out_path
function(vt_set_module_default_target_properties_out_path TARGET_NAME OUTPUT_PATH)
	set_target_properties(${TARGET_NAME} PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY $<1:${OUTPUT_PATH}>
		LIBRARY_OUTPUT_DIRECTORY $<1:${OUTPUT_PATH}>
		RUNTIME_OUTPUT_DIRECTORY $<1:${OUTPUT_PATH}>
		OUTPUT_NAME "${PROJECT_NAME}_$<CONFIG>_${CMAKE_GENERATOR_PLATFORM}"
		CXX_STANDARD 17
	)

	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	if ((MSVC) AND (DEFINED GLOBAL_DEBUG_PATH)) 
		set_property(
			TARGET ${TARGET_NAME}
			PROPERTY
				VS_DEBUGGER_WORKING_DIRECTORY ${GLOBAL_DEBUG_PATH}
		)
	endif()

	if(DEFINED VT_MODULE_PATH)
		set_target_properties(${TARGET_NAME} PROPERTIES
			FOLDER ${VT_MODULE_PATH}
		)
	endif()

	get_target_property(TARGET_TYPE ${TARGET_NAME} TYPE)
	set(INCLUDE_TYPE)
	if (${TARGET_TYPE} STREQUAL "INTERFACE_LIBRARY")
		set(INCLUDE_TYPE INTERFACE)
	else()
		set(INCLUDE_TYPE PUBLIC)
	endif()

	target_include_directories(${TARGET_NAME}
		${INCLUDE_TYPE}
			${CMAKE_CURRENT_LIST_DIR}/Sources
	)
endfunction()

### vt_add_module_target_name
function(vt_add_module_target_name CURRENT_TARGET_NAME CURRENT_MODULE_NAME LINK_MODIFIER FOLDER_NAME OUTPUT_PATH)
	set(ALL_SOURCE)
	vt_get_all_sources_recurse("${CMAKE_CURRENT_LIST_DIR}/Sources/${CURRENT_MODULE_NAME}" "${CURRENT_MODULE_NAME}" ALL_SOURCE)
	vt_generate_source_groups_by_sources("${CMAKE_CURRENT_LIST_DIR}/Sources/${CURRENT_MODULE_NAME}" "${ALL_SOURCE}")

	add_library(${CURRENT_TARGET_NAME} ${LINK_MODIFIER} ${ALL_SOURCE})

	vt_set_module_default_target_properties_out_path(${CURRENT_TARGET_NAME} ${OUTPUT_PATH})
	set_target_properties(${CURRENT_TARGET_NAME} PROPERTIES
		FOLDER ${FOLDER_NAME}
	)
endfunction()

#########

### vt_link_module
macro(vt_link_module CURRENT_TARGET_NAME LINK_MODIFIER MODULE_NAME)
	target_link_libraries(${CURRENT_TARGET_NAME} ${LINK_MODIFIER} ${MODULE_NAME})
endmacro()

### vt_add_module_internal
function(vt_add_module_internal CURRENT_TARGET_NAME LINK_MODIFIER MODULE MODULE_PATH BINARY_DIR)
	add_subdirectory(${MODULE_PATH} ${BINARY_DIR}/${MODULE})
	vt_link_module(${CURRENT_TARGET_NAME} ${LINK_MODIFIER} ${MODULE})
endfunction()

### vt_add_module
function(vt_add_module CURRENT_TARGET_NAME LINK_MODIFIER MODULE MODULE_PATH BINARY_DIR)
	message(">>> adding module: ${VT_COLOUR_BOLD_MAGENTA}${MODULE}${VT_COLOUR_RESET}")
	vt_add_module_internal(${CURRENT_TARGET_NAME} ${LINK_MODIFIER} ${MODULE} ${MODULE_PATH}/${MODULE} ${BINARY_DIR})
endfunction()

### vt_add_module_by_path
function(vt_add_module_by_path CURRENT_TARGET_NAME LINK_MODIFIER MODULE_NAME MODULE_PATH BINARY_DIR)
	message(">>> adding module: ${VT_COLOUR_BOLD_MAGENTA}${MODULE_NAME}${VT_COLOUR_RESET} (${MODULE_PATH})")
	vt_add_module_internal(${CURRENT_TARGET_NAME} ${LINK_MODIFIER} ${MODULE_NAME} ${MODULE_PATH} ${BINARY_DIR})
endfunction()

### vt_add_modules
function(vt_add_modules CURRENT_TARGET_NAME LINK_MODIFIER MODULES_SET MODULES_PATH BINARY_DIR)
	foreach(MODULE ${MODULES_SET})
		vt_add_module(${CURRENT_TARGET_NAME} ${LINK_MODIFIER} ${MODULE} ${MODULES_PATH} ${BINARY_DIR})
	endforeach()
endfunction()

### vt_add_modules_include
function(vt_add_modules_include CURRENT_TARGET_NAME LINK_MODIFIER MODULES_SET_PATH MODULES_PATH BINARY_DIR)
	include(${MODULES_SET_PATH})

	if (DEFINED MODULES_SET)
		vt_add_modules(${CURRENT_TARGET_NAME} ${LINK_MODIFIER} "${MODULES_SET}" ${MODULES_PATH} ${BINARY_DIR})
	endif()
endfunction()

### vt_add_modules_by_path
function(vt_add_modules_by_path CURRENT_TARGET_NAME LINK_MODIFIER MODULES_SET MODULES_PATH BINARY_DIR)
	vt_check_root_path()

	include(${VT_ROOT_PATH}/CMake/Array2d.cmake)

	array2d_begin_loop( advanced "${MODULES_SET}" 2 "MODULE_NAME;MODULE_PATH" )
	while( advanced )
		vt_add_module_by_path(${CURRENT_TARGET_NAME} ${LINK_MODIFIER} ${MODULE_NAME} ${MODULES_PATH}/${MODULE_PATH} ${BINARY_DIR})
		array2d_advance()
	endwhile()
endfunction()

### vt_add_modules_by_path_include
function(vt_add_modules_by_path_include CURRENT_TARGET_NAME LINK_MODIFIER MODULES_SET_PATH MODULES_PATH BINARY_DIR)
	include(${MODULES_SET_PATH})

	if (DEFINED MODULES_SET)
		vt_add_modules_by_path(${CURRENT_TARGET_NAME} ${LINK_MODIFIER} "${MODULES_SET}" ${MODULES_PATH} ${BINARY_DIR})
	endif()
endfunction()

########

### vt_add_plugin
function(vt_add_plugin CURRENT_TARGET_NAME PLUGINS_SET PLUGINS_PATH BINARY_DIR)
	foreach(PLUGIN ${PLUGINS_SET})
		message(">>> adding plugin: ${VT_COLOUR_BOLD_BLUE}${PLUGIN}${VT_COLOUR_RESET}")
		add_subdirectory(${PLUGINS_PATH}/${PLUGIN} ${BINARY_DIR}/${PLUGIN})
	endforeach()
endfunction()

### vt_add_plugins_include
function(vt_add_plugins_include CURRENT_TARGET_NAME PLUGINS_SET_PATH PLUGINS_PATH BINARY_DIR)
	include(${PLUGINS_SET_PATH})

	if (DEFINED PLUGINS_SET)
		vt_add_plugin(${CURRENT_TARGET_NAME} "${PLUGINS_SET}" ${PLUGINS_PATH} ${BINARY_DIR})
	endif()
endfunction()

### vt_add_plugins_by_path
function(vt_add_plugins_by_path CURRENT_TARGET_NAME PLUGINS_SET PLUGINS_PATH BINARY_DIR)
	vt_check_root_path()

	include(${VT_ROOT_PATH}/CMake/Array2d.cmake)

	array2d_begin_loop(advanced "${PLUGINS_SET}" 2 "PLUGIN_NAME;PLUGIN_PATH")
	while(advanced)
		message(">>> adding plugin: ${VT_COLOUR_BOLD_BLUE}${PLUGIN_NAME}${VT_COLOUR_RESET} (${PLUGIN_PATH})")
		add_subdirectory(${PLUGIN_PATH}/${PLUGIN_NAME} ${BINARY_DIR}/${PLUGIN_NAME})
		array2d_advance()
	endwhile()
endfunction()

### vt_add_plugins_by_path_include
function(vt_add_plugins_by_path_include CURRENT_TARGET_NAME PLUGINS_SET_PATH PLUGINS_PATH BINARY_DIR)
	include(${PLUGINS_SET_PATH})

	if (DEFINED PLUGINS_SET)
		vt_add_plugins_by_path(${CURRENT_TARGET_NAME} "${PLUGINS_SET}" ${PLUGINS_PATH} ${BINARY_DIR})
	endif()
endfunction()

########

### vt_add_third_party_module
function(vt_add_third_party_module CURRENT_TARGET_NAME LINK_MODIFIER THIRD_PARTY_MODULE_NAME TARGETS)
	include(${VT_ROOT_PATH}/CMake/PreBuild.cmake)

	vt_check_and_build_module(${THIRD_PARTY_MODULE_NAME})
	if ((NOT ${${THIRD_PARTY_MODULE_NAME}_FOUND}) OR (NOT DEFINED ${THIRD_PARTY_MODULE_NAME}_FOUND))
		find_package(${THIRD_PARTY_MODULE_NAME} REQUIRED)
		if ((NOT ${${THIRD_PARTY_MODULE_NAME}_FOUND}) OR (NOT DEFINED ${THIRD_PARTY_MODULE_NAME}_FOUND))
			message(FATAL_ERROR "Error: '${THIRD_PARTY_MODULE_NAME}' hasn't been found.")
		endif()
	endif()

	foreach(TARGET ${TARGETS})
		message(">>> adding third party module: ${VT_COLOUR_BOLD_YELLOW}${TARGET}${VT_COLOUR_RESET} (${THIRD_PARTY_MODULE_NAME})")
		vt_link_module(${CURRENT_TARGET_NAME} ${LINK_MODIFIER} ${TARGET})
	endforeach()
endfunction()

### vt_add_third_party_modules
function(vt_add_third_party_modules CURRENT_TARGET_NAME LINK_MODIFIER THIRD_PARTY_MODULES_SET)
	vt_check_root_path()

	include(${VT_ROOT_PATH}/CMake/Array2d.cmake)

	array2d_begin_loop( advanced "${THIRD_PARTY_MODULES_SET}" 2 "THIRD_PARTY_MODULE_NAME;TARGETS" )
	while( advanced )
		vt_add_third_party_module(${CURRENT_TARGET_NAME} ${LINK_MODIFIER} ${THIRD_PARTY_MODULE_NAME} ${TARGETS})
		array2d_advance()
	endwhile()
endfunction()

### vt_add_third_party_modules_include
function(vt_add_third_party_modules_include CURRENT_TARGET_NAME LINK_MODIFIER THIRD_PARTY_MODULES_SET_PATH)
	include(${THIRD_PARTY_MODULES_SET_PATH})

	if (DEFINED THIRD_PARTY_MODULES_SET)
		vt_add_third_party_modules(${CURRENT_TARGET_NAME} ${LINK_MODIFIER} "${THIRD_PARTY_MODULES_SET}")
	endif()
endfunction()

########

### vt_add_module_target
function(vt_add_module_target CURRENT_MODULE_NAME LINK_MODIFIER FOLDER_NAME)
	vt_add_module_target_name(${CURRENT_MODULE_NAME} ${CURRENT_MODULE_NAME} ${LINK_MODIFIER} ${FOLDER_NAME} ${CMAKE_CURRENT_BINARY_DIR}/Out)
endfunction()

### vt_add_module_target_out_path
function(vt_add_module_target_out_path CURRENT_MODULE_NAME LINK_MODIFIER FOLDER_NAME OUTPUT_PATH)
	vt_add_module_target_name(${CURRENT_MODULE_NAME} ${CURRENT_MODULE_NAME} ${LINK_MODIFIER} ${FOLDER_NAME} ${OUTPUT_PATH})
endfunction()

### vt_add_common_module_target
function(vt_add_common_module_target CURRENT_MODULE_NAME LINK_MODIFIER FOLDER_NAME)
	vt_add_module_target_name(${CURRENT_MODULE_NAME}_COMMON ${CURRENT_MODULE_NAME} ${LINK_MODIFIER} ${FOLDER_NAME} ${CMAKE_CURRENT_BINARY_DIR}/Out)
endfunction()

### vt_add_common_module_target_out_path
function(vt_add_common_module_target_out_path CURRENT_MODULE_NAME LINK_MODIFIER FOLDER_NAME OUTPUT_PATH)
	vt_add_common_module_target_out_path(${CURRENT_MODULE_NAME}_COMMON ${CURRENT_MODULE_NAME} ${LINK_MODIFIER} ${FOLDER_NAME} ${OUTPUT_PATH})
endfunction()

### vt_add_ecs_module_target
function(vt_add_ecs_module_target CURRENT_MODULE_NAME LINK_MODIFIER FOLDER_NAME)
	vt_add_module_target_name(${CURRENT_MODULE_NAME}_ECS ${CURRENT_MODULE_NAME} ${LINK_MODIFIER} ${FOLDER_NAME} ${CMAKE_CURRENT_BINARY_DIR}/Out)
endfunction()

### vt_add_plugin_target
function(vt_add_plugin_target CURRENT_MODULE_NAME LINK_MODIFIER FOLDER_NAME)
	vt_add_module_target(${CURRENT_MODULE_NAME} ${LINK_MODIFIER} ${FOLDER_NAME})
endfunction()

### vt_add_plugin_target_out_path
function(vt_add_plugin_target_out_path CURRENT_MODULE_NAME LINK_MODIFIER FOLDER_NAME OUTPUT_PATH)
	vt_add_module_target_out_path(${CURRENT_MODULE_NAME} ${LINK_MODIFIER} ${FOLDER_NAME} ${OUTPUT_PATH})
endfunction()

### vt_add_common_module_out_path
function(vt_add_common_module_out_path CURRENT_MODULE_NAME OUT_PATH)
	vt_add_module_by_path(${CURRENT_MODULE_NAME} PUBLIC ${CURRENT_MODULE_NAME}_COMMON ${CMAKE_CURRENT_LIST_DIR}/../Common ${OUT_PATH})
endfunction()

### vt_add_common_module
function(vt_add_common_module CURRENT_MODULE_NAME)
	vt_add_common_module_out_path(${CURRENT_MODULE_NAME} ${CMAKE_CURRENT_BINARY_DIR}/../${CURRENT_MODULE_NAME}_COMMON)
endfunction()

### vt_add_ecs_module
function(vt_add_ecs_module CURRENT_MODULE_NAME)
	vt_add_module_by_path(${CURRENT_MODULE_NAME} PUBLIC ${CURRENT_MODULE_NAME}_ECS ${CMAKE_CURRENT_LIST_DIR}/../ECS ${CMAKE_CURRENT_BINARY_DIR}/../${CURRENT_MODULE_NAME}_ECS)
endfunction()
