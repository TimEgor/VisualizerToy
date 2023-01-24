cmake_minimum_required(VERSION 3.8)

include(${VT_ROOT_PATH}/CMake/Module.cmake)

project(GraphicDeviceModule)

if (NOT DEFINED VT_COMMON_ENGINE)
	add_library(GraphicDeviceModule INTERFACE)

	vt_add_modules_by_path_include(GraphicDeviceModule INTERFACE "DeviceModules.cmake"
	${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/Modules)
else()
	add_library(GraphicDeviceModule_COMMON INTERFACE)

	vt_add_modules_by_path_include(GraphicDeviceModule_COMMON INTERFACE "CommonDeviceModules.cmake"
		${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/Modules)
endif()