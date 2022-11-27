cmake_minimum_required(VERSION 3.8)

include(${VT_ROOT_PATH}/CMake/Module.cmake)

project(ResourceModule)

add_library(ResourceModule INTERFACE)

vt_add_modules_include(ResourceModule INTERFACE "ResourceModules.cmake"
	${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/Modules
)