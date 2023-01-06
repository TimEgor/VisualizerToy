cmake_minimum_required(VERSION 3.8)

project(ModelConverterFBX)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

set(CONVERTER_OUT ${VT_ASSET_CONVERTERS_OUT})

vt_add_plugin_target_out_path(ModelConverterFBX SHARED "VT_Converters" ${CONVERTER_OUT})

vt_add_third_party_module(ModelConverterFBX PRIVATE "FBXSDK" "FBXSDK")

target_link_libraries(ModelConverterFBX
	PUBLIC
		ConverterCommon
		Math
		InputLayout
		MeshSystem
)