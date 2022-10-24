set(PUGIXML_ROOT_PATH ${P3D_ROOT_PATH}/Libs/Install/${CMAKE_GENERATOR_PLATFORM}/pugiXML)

set(PUGIXML_INCLUDE_DIR ${PUGIXML_ROOT_PATH}/include)

set(PUGIXML_LIBRARY_RELEASE ${PUGIXML_ROOT_PATH}/lib/pugixml.lib)
set(PUGIXML_LIBRARY_DEBUG ${PUGIXML_ROOT_PATH}/lib/pugixml_d.lib)

if((EXISTS ${PUGIXML_LIBRARY_RELEASE}) AND (EXISTS ${PUGIXML_LIBRARY_DEBUG}))
	include(SelectLibraryConfigurations)
	select_library_configurations(PUGIXML)
endif()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(pugixml DEFAULT_MSG
	PUGIXML_LIBRARY PUGIXML_INCLUDE_DIR)

if(pugixml_FOUND)
	if(NOT TARGET pugixml::static)
		add_library(pugixml::static UNKNOWN IMPORTED)
		set_target_properties(pugixml::static PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES "${PUGIXML_INCLUDE_DIR}"
			IMPORTED_LOCATION "${PUGIXML_LIBRARY_RELEASE}"
			IMPORTED_LOCATION_DEBUG "${PUGIXML_LIBRARY_DEBUG}"
		)
	endif()
endif()