cmake_minimum_required(VERSION 3.8)

project(MeshSystemCommon)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_common_module_target(MeshSystem INTERFACE "VT_Engine/Graphics/Renderer/MeshSystem")

target_link_libraries(MeshSystem_COMMON
	INTERFACE
		GraphicResourceManager_COMMON
		GraphicResourceCommon
		GPUBuffer
)