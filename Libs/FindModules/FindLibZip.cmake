set(LIBZIP_ROOT_PATH ${P3D_ROOT_PATH}/Libs/Install/${CMAKE_GENERATOR_PLATFORM}/LibZip)

set(LIBZIP_INCLUDE_DIR ${LIBZIP_ROOT_PATH}/include)

set(LIBZIP_LIBRARY_RELEASE ${LIBZIP_ROOT_PATH}/lib/zip.lib)
set(LIBZIP_LIBRARY_DEBUG ${LIBZIP_ROOT_PATH}/lib/zip_d.lib)

if((EXISTS ${LIBZIP_LIBRARY_RELEASE}) AND (EXISTS ${LIBZIP_LIBRARY_DEBUG}))
	include(SelectLibraryConfigurations)
	select_library_configurations(LIBZIP)
endif()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(libzip DEFAULT_MSG
	LIBZIP_LIBRARY LIBZIP_INCLUDE_DIR)

if(libzip_FOUND)
	if(NOT TARGET libzip::zip)
		add_library(libzip::zip UNKNOWN IMPORTED)
		set_target_properties(libzip::zip PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES "${LIBZIP_INCLUDE_DIR}"
			IMPORTED_LOCATION "${LIBZIP_LIBRARY_RELEASE}"
			IMPORTED_LOCATION_DEBUG "${LIBZIP_LIBRARY_DEBUG}"
		)
	endif()
endif()