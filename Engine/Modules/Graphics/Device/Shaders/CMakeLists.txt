cmake_minimum_required(VERSION 3.8)

project(Shaders)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(Shaders INTERFACE "VT_Engine/Graphics/Device")

target_link_libraries(Shaders
	INTERFACE
        GraphicResourceCommon
)