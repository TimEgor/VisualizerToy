cmake_minimum_required(VERSION 3.8)

project(Profile)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)
include(${VT_ROOT_PATH}/CMake/PreBuild.cmake)

vt_add_module_target(Profile STATIC "VT_Engine")

target_link_libraries(Profile
	PUBLIC
		Core
)

vt_check_and_build_module(WinPixEventRuntime FALSE)
if ((${WinPixEventRuntime_FOUND}) AND (DEFINED WinPixEventRuntime_FOUND))
	target_compile_definitions(Profile PRIVATE VT_PROFILE_PIX)
	vt_add_third_party_module(Profile PRIVATE WinPixEventRuntime WinPixEventRuntime)
endif()

vt_check_and_build_module(SuperluminalAPI FALSE)
if ((${SuperluminalAPI_FOUND}) AND (DEFINED SuperluminalAPI_FOUND))
	target_compile_definitions(Profile PRIVATE VT_PROFILE_SUPERLUMINAL)
	vt_add_third_party_module(Profile PRIVATE SuperluminalAPI SuperluminalAPI)
endif()