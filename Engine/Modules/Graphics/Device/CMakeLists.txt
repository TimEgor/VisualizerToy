cmake_minimum_required(VERSION 3.8)

include(${VT_ROOT_PATH}/CMake/Module.cmake)

project(GraphicDeviceModule)

add_library(GraphicDeviceModule INTERFACE)

vt_add_modules_include(GraphicDeviceModule INTERFACE "DeviceModules.cmake"
	${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/Modules
)