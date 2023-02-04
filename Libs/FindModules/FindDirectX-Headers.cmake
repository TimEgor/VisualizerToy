set(DirectX-Headers_DIR ${VT_ROOT_PATH}/Libs/Install/${CMAKE_GENERATOR_PLATFORM}/DirectX-Headers/share/directx-headers/cmake)
if(EXISTS ${DirectX-Headers_DIR})
	find_package(DirectX-Headers REQUIRED CONFIG)
endif()