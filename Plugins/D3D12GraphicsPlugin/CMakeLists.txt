cmake_minimum_required(VERSION 3.8)

project(D3D12GraphicsPlugin)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_plugin_target_out_path(D3D12GraphicsPlugin SHARED "VT_Plugins" ${VT_PLUGIN_OUT})

target_link_libraries(D3D12GraphicsPlugin
	PUBLIC
		Core

		GraphicPlatform
		ManagedGraphicDevice

		RenderSystem_COMMON

		PluginSystem_COMMON
		WindowSystem_COMMON
		Platform

		d3d12
		dxgi
		dxguid
)