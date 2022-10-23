cmake_minimum_required(VERSION 3.8)

include(${VT_ROOT_PATH}/CMake/Module.cmake)

project(Graphics)

add_library(Graphics INTERFACE)

set(GRAPHICS_MODULES_SET
	Device
)

vt_add_modules(Graphics INTERFACE "${GRAPHICS_MODULES_SET}"
	${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/Modules
)