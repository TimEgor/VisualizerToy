cmake_minimum_required(VERSION 3.8)

project(GraphicPipeline)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(GraphicPipeline INTERFACE "VT_Engine/Graphics/Device")

target_link_libraries(GraphicPipeline
	INTERFACE
		GraphicResourceCommon
		Core
)