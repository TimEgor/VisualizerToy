if (NOT DEFINED VT_OPTIONS_SET)
	set(VT_OPTIONS_SET)

	if (NOT DEFINED VT_ROOT_PATH)
		message(FATAL_ERROR "Error: 'VT_ROOT_PATH' hasn't been defined.")
	endif()

	add_compile_definitions(VT_ROOT_PATH="${VT_ROOT_PATH}")

	if(DEFINED SIMD_MATH)
		if (SIMD_MATH STREQUAL "")
			message(WARNING "Specified empty SIMD math realization.")
		else()
			add_compile_definitions(VT_SIMD_MATH_${SIMD_MATH})
		endif()
	endif()
	
	if(DEFINED IMGUI)
		add_compile_definitions(VT_IMGUI)
	endif()

	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${VT_ROOT_PATH}/Libs/FindModules)
endif() #VT_OPTIONS_SET