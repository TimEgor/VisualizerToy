cmake_minimum_required(VERSION 3.8)

project(GraphicPresenterCommon)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_common_module_target(GraphicPresenter INTERFACE "VT_Engine/Graphics/Presenter")

target_link_libraries(GraphicPresenter_COMMON
	INTERFACE
		Textures
)