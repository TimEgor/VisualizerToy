set(EnTT_DIR ${VT_ROOT_PATH}/Libs/Install/${CMAKE_GENERATOR_PLATFORM}/EnTT/lib/EnTT/cmake)
if(EXISTS ${EnTT_DIR})
	find_package(EnTT REQUIRED CONFIG)
endif()