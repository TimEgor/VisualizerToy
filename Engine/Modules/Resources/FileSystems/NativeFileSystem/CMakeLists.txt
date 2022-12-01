cmake_minimum_required(VERSION 3.8)

project(NativeFileSystem)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(NativeFileSystem STATIC "VT_Engine/Resources/FileSystems")

target_link_libraries(NativeFileSystem
	PUBLIC
		FileSystem
)