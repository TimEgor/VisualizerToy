cmake_minimum_required(VERSION 3.8)

include(${VT_ROOT_PATH}/CMake/Module.cmake)

project(RendererModule)

if (NOT DEFINED VT_COMMON_ENGINE)
	add_library(RendererModule INTERFACE)

	vt_add_modules_by_path_include(RendererModule INTERFACE "RendererModules.cmake"
	${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/Modules)
else()
	add_library(RendererModule_COMMON INTERFACE)

	vt_add_modules_by_path_include(RendererModule_COMMON INTERFACE "CommonRendererModules.cmake"
		${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/Modules)
endif()