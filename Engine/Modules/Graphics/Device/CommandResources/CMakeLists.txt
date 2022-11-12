cmake_minimum_required(VERSION 3.8)

project(GraphicCommandResources)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(GraphicCommandResources INTERFACE "VT_Engine/Graphics/Device")

#target_link_libraries(GraphicCommandResources
#	INTERFACE
#		Core
#)