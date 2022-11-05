set(volk_DIR ${VT_ROOT_PATH}/Libs/Install/${CMAKE_GENERATOR_PLATFORM}/Volk/lib/cmake/volk)
if(EXISTS ${volk_DIR})
	find_package(volk REQUIRED CONFIG)
endif()