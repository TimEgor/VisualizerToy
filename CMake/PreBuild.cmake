### vt_build_module
function(vt_build_module MODULE_NAME REQUIRING_STATE)
	message("\n=========")
	message(">>> Building: ${CMAKE_GENERATOR_PLATFORM} ${MODULE_NAME}\n")

	set(MODULE_PATH ${VT_ROOT_PATH}/Libs/${MODULE_NAME})

	if (NOT EXISTS ${MODULE_PATH})
		if (${REQUIRING_STATE})
			message(FATAL_ERROR "Error: Module '${MODULE_PATH}' hasn't been found.")
		else()
			return()
		endif()
	endif()

	set(MODULE_NEED_INSTALL FALSE)

	set(THIRD_PARTY_MODULE_BUILD_ARGS_PATH ${VT_ROOT_PATH}/Libs/BuildArgs/${MODULE_NAME}.cmake)
	if (EXISTS ${THIRD_PARTY_MODULE_BUILD_ARGS_PATH})
		include(${THIRD_PARTY_MODULE_BUILD_ARGS_PATH})
	endif()

	set(MODULE_BINARY_PATH ${MODULE_PATH}/Build)
	if (EXISTS ${MODULE_BINARY_PATH})
		file(REMOVE_RECURSE ${MODULE_BINARY_PATH})
	endif()

	file(MAKE_DIRECTORY ${MODULE_BINARY_PATH})

	execute_process(COMMAND ${CMAKE_COMMAND} -A ${CMAKE_GENERATOR_PLATFORM} ${MODULE_PATH} ${MODULE_CONFIG_ARGS}
		-DCMAKE_INSTALL_PREFIX=${VT_ROOT_PATH}/Libs/Install/${CMAKE_GENERATOR_PLATFORM}/${MODULE_INSTALL_DIR}
		WORKING_DIRECTORY ${MODULE_BINARY_PATH}
	)

	if (NOT DEFINED PREBUILD_CONFIGURATIONS)
		set(PREBUILD_CONFIGURATIONS "Debug" "Release")
	endif()

	foreach(CONFIGURATION ${PREBUILD_CONFIGURATIONS})
		execute_process(COMMAND ${CMAKE_COMMAND} --build . --config ${CONFIGURATION}
			WORKING_DIRECTORY ${MODULE_BINARY_PATH}
		)
	endforeach()

	if (${MODULE_NEED_INSTALL})
		message(">>> Installing dir: ${MODULE_INSTALL_DIR}")
		foreach(CONFIGURATION ${PREBUILD_CONFIGURATIONS})
			execute_process(COMMAND ${CMAKE_COMMAND} --install . --config ${CONFIGURATION}
				WORKING_DIRECTORY ${MODULE_BINARY_PATH}
			)
		endforeach()
	endif()

	message("=========")
endfunction()

### vt_check_and_build_module
macro(vt_check_and_build_module MODULE_NAME REQUIRING_STATE)
	if ((NOT ${${MODULE_NAME}_FOUND}) OR (NOT DEFINED ${MODULE_NAME}_FOUND))
		find_package(${MODULE_NAME})

		if ((NOT ${${MODULE_NAME}_FOUND}) OR (NOT DEFINED ${MODULE_NAME}_FOUND))
			message(">>> Find module - FAILED : '${MODULE_NAME}'")
			vt_build_module(${MODULE_NAME} ${REQUIRING_STATE})
			find_package(${MODULE_NAME})
		endif()
	endif()
endmacro()