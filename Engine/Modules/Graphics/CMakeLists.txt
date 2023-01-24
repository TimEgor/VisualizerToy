cmake_minimum_required(VERSION 3.8)

include(${VT_ROOT_PATH}/CMake/Module.cmake)

project(GraphicsModule)

if (NOT DEFINED VT_COMMON_ENGINE)
	add_library(GraphicsModule INTERFACE)

	vt_add_modules_by_path_include(GraphicsModule INTERFACE "GraphicModules.cmake"
	${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/Modules)
else()
	add_library(GraphicsModule_COMMON INTERFACE)

	vt_add_modules_by_path_include(GraphicsModule_COMMON INTERFACE "CommonGraphicModules.cmake"
		${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/Modules)
endif()