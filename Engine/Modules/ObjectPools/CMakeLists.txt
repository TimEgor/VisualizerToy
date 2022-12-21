cmake_minimum_required(VERSION 3.8)

include(${VT_ROOT_PATH}/CMake/Module.cmake)

project(ObjectPoolsModule)

add_library(ObjectPoolsModule INTERFACE)

vt_add_modules_include(ObjectPoolsModule INTERFACE "PoolsModules.cmake"
	${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/Modules
)