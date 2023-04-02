cmake_minimum_required(VERSION 3.8)

include(${VT_ROOT_PATH}/CMake/Module.cmake)

project(DebuggingModule)

if (NOT DEFINED VT_COMMON_ENGINE)
	add_library(DebuggingModule INTERFACE)

	vt_add_modules_by_path_include(DebuggingModule INTERFACE "DebuggingModules.cmake"
	${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/Modules)
else()
	add_library(DebuggingModule_COMMON INTERFACE)

	vt_add_modules_by_path_include(DebuggingModule_COMMON INTERFACE "CommonDebuggingModules.cmake"
		${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/Modules)
endif()