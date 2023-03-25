cmake_minimum_required(VERSION 3.8)

include(${VT_ROOT_PATH}/CMake/Module.cmake)

project(UtilsModule)

if (NOT DEFINED VT_COMMON_ENGINE)
	add_library(UtilsModule INTERFACE)

	vt_add_modules_by_path_include(UtilsModule INTERFACE "UtilsModules.cmake"
	${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/Modules)
else()
	add_library(UtilsModule_COMMON INTERFACE)

	vt_add_modules_by_path_include(UtilsModule_COMMON INTERFACE "CommonUtilsModules.cmake"
		${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/Modules)
endif()