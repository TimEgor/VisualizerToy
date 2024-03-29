cmake_minimum_required(VERSION 3.8)

project(D3D12GraphicsPlugin)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_plugin_target_out_path(D3D12GraphicsPlugin SHARED "VT_Plugins" ${VT_PLUGIN_OUT})

vt_add_third_party_module(D3D12GraphicsPlugin PRIVATE "D3D12MemoryAllocator" "D3D12MemoryAllocator")
vt_add_third_party_module(D3D12GraphicsPlugin PRIVATE "DirectX-Headers" "Microsoft::DirectX-Headers")

target_link_libraries(D3D12GraphicsPlugin
	PRIVATE
		Profile

	PUBLIC
		Core

		GraphicPlatform
		ManagedGraphicDevice

		RenderSystem_COMMON
		RenderContext

		PluginSystem_COMMON
		WindowSystem_COMMON
		Platform

		ImGui

		d3d12
		dxgi
		dxguid
		D3DCompiler
)