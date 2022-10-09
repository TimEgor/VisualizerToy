cmake_minimum_required(VERSION 3.8)

project(PluginSystemCommon)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_common_module_target(PluginSystem INTERFACE "VT_Engine/PluginSystem/")

target_link_libraries(PluginSystem_COMMON
	INTERFACE
        Core
)