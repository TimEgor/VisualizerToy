if(${CMAKE_GENERATOR_PLATFORM} MATCHES x64)
	set(PIX_DIR ${VT_ROOT_PATH}/Libs/WinPixEventRuntime)
	
	include(SelectLibraryConfigurations)
	select_library_configurations(WinPixEventRuntime)
	
	set(PIX_INCLUDE ${PIX_DIR}/Include)
	set(PIX_BIN_DIR ${PIX_DIR}/Bin/x64)
	set(PIX_BINARY ${PIX_BIN_DIR}/WinPixEventRuntime.dll)
	set(PIX_LIBRARY ${PIX_BIN_DIR}/WinPixEventRuntime.lib)

	include(FindPackageHandleStandardArgs)
	FIND_PACKAGE_HANDLE_STANDARD_ARGS(WinPixEventRuntime
		REQUIRED_VARS PIX_INCLUDE PIX_BINARY PIX_LIBRARY
	)

	if(WinPixEventRuntime_FOUND)
		if(NOT TARGET WinPixEventRuntime)
			add_library(WinPixEventRuntime INTERFACE)

			set_target_properties(WinPixEventRuntime PROPERTIES
				INTERFACE_INCLUDE_DIRECTORIES ${PIX_INCLUDE}
			)

			target_link_libraries(WinPixEventRuntime
				INTERFACE ${PIX_LIBRARY}
			)
		endif()
	endif()
endif()