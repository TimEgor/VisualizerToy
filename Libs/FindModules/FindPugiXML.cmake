set(PugiXML_DIR ${VT_ROOT_PATH}/Libs/Install/${CMAKE_GENERATOR_PLATFORM}/PugiXML/lib/cmake/PugiXML)
if(EXISTS ${PugiXML_DIR})
	find_package(PugiXML REQUIRED CONFIG)

	if (TARGET pugixml::static)
		set_target_properties(pugixml::static PROPERTIES MAP_IMPORTED_CONFIG_PROFILE RELWITHDEBINFO)
		set_target_properties(pugixml::static PROPERTIES MAP_IMPORTED_CONFIG_RELEASE RELWITHDEBINFO)
	endif()
endif()