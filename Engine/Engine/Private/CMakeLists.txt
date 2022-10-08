cmake_minimum_required(VERSION 3.8)

project(Engine)

set(VT_ROOT_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)
include(${VT_ROOT_PATH}/CMake/BuildingOptions.cmake)

vt_add_module_target(Engine SHARED "VT_Engine/Engine")
vt_add_common_module_out_path(Engine ${CMAKE_CURRENT_BINARY_DIR}/Engine_COMMON)

target_link_libraries(Engine
	PUBLIC
		Engine_COMMON
)

vt_add_modules_by_path_include(Engine PRIVATE
	"EngineModules.cmake" ${CMAKE_CURRENT_LIST_DIR}/../../Modules ${CMAKE_CURRENT_BINARY_DIR}/Modules)