if (NOT DEFINED OPTIONS_SET)
	if (NOT DEFINED VT_ROOT_PATH)
		message(FATAL_ERROR "Error: 'VT_ROOT_PATH' hasn't been defined.")
	endif()

	add_compile_definitions(VT_ROOT_PATH="${VT_ROOT_PATH}")
	add_compile_definitions(VT_CONFIG=$<CONFIG> VT_CONFIG_NAME="$<CONFIG>")

	if(DEFINED SIMD_MATH)
		if (SIMD_MATH STREQUAL "")
			message(WARNING "Specified empty SIMD math realization.")
		else()
			add_compile_definitions(SIMD_MATH_${SIMD_MATH})
		endif()
	endif()
	
	if(DEFINED IMGUI)
		add_compile_definitions(IMGUI)
	endif()

	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${VT_ROOT_PATH}/Libs/FindModules)

	set(OPTIONS_SET)
endif() #OPTIONS_SET