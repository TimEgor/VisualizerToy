cmake_minimum_required(VERSION 3.8)

include(${VT_ROOT_PATH}/CMake/Module.cmake)

project(GraphicsModule)

add_library(GraphicsModule INTERFACE)

set(GRAPHICS_MODULES_SET
	GraphicDeviceModule	Device
)

vt_add_modules_by_path(GraphicsModule INTERFACE "${GRAPHICS_MODULES_SET}"
	${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/Modules
)