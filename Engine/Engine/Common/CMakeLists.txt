cmake_minimum_required(VERSION 3.8)

project(EngineCommon)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_common_module_target(Engine INTERFACE "VT_Engine/Engine/")

target_link_libraries(Engine_COMMON
	INTERFACE
		Core
)

if (NOT DEFINED VT_ENGINE)
	set(VT_COMMON_ENGINE TRUE)
	message("\n>>>>>> MODULES <<<<<<<")
	vt_add_modules_by_path_include(Engine_COMMON INTERFACE
		"CommonEngineModules.cmake" ${CMAKE_CURRENT_LIST_DIR}/../../Modules ${CMAKE_CURRENT_BINARY_DIR}/Modules)

	if (DEFINED VT_ENGINE_ECS)
		vt_add_modules_by_path_include(Engine_COMMON INTERFACE
			"ECSEngineModules.cmake" ${CMAKE_CURRENT_LIST_DIR}/../../Modules ${CMAKE_CURRENT_BINARY_DIR}/Modules)
	endif()
endif()