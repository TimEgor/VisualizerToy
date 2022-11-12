cmake_minimum_required(VERSION 3.8)

project(ManagedGraphicDevice)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(ManagedGraphicDevice STATIC "VT_Engine/Graphics/Device")

target_link_libraries(ManagedGraphicDevice
	PUBLIC
		GraphicDevice
		SwapChain
		Textures
	PRIVATE
		ObjectPool
)