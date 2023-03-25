set(PugiXML_DIR ${VT_ROOT_PATH}/Libs/Install/${CMAKE_GENERATOR_PLATFORM}/PugiXML/lib/cmake/PugiXML)
if(EXISTS ${PugiXML_DIR})
	find_package(PugiXML REQUIRED CONFIG)
endif()