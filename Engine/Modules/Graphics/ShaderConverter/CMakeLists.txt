cmake_minimum_required(VERSION 3.8)

project(ShaderConverter)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(ShaderConverter INTERFACE "VT_Engine/Graphics")

target_link_libraries(ShaderConverter
	INTERFACE
		ResourceConverter
		Shaders
)