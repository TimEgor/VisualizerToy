cmake_minimum_required(VERSION 3.8)

project(ProjectLoader)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(ProjectLoader STATIC "VT_Engine/Utils")

target_link_libraries(ProjectLoader
	PUBLIC
		Core
	PRIVATE
		Engine_COMMON
		ResourceSystem
		GameSystem_COMMON
		XmlParser
)