cmake_minimum_required(VERSION 3.8)

project(GraphicPresenter)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(GraphicPresenter STATIC "VT_Engine/Graphics/Presenter")
vt_add_common_module(GraphicPresenter)

target_link_libraries(GraphicPresenter
	PRIVATE
		GraphicDevice
		Profile

	PUBLIC
		Engine_COMMON
		SwapChain
		WindowSystem_COMMON
)