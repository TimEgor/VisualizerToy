cmake_minimum_required(VERSION 3.8)

project(VulkanGraphicsPlugin)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_plugin_target_out_path(VulkanGraphicsPlugin SHARED "VT_Plugins" ${VT_PLUGIN_OUT})

vt_add_third_party_module(VulkanGraphicsPlugin PRIVATE "volk" "volk::volk")

target_link_libraries(VulkanGraphicsPlugin
	PUBLIC
		Core
		GraphicDevice
		GraphicResourceManager_COMMON
		SwapChain
		Textures

		PluginSystem_COMMON
		WindowSystem_COMMON
		Platform
)