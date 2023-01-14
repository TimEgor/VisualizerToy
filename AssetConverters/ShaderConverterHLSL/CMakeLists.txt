cmake_minimum_required(VERSION 3.8)

project(ShaderConverterHLSL)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)
include(${VT_ROOT_PATH}/CMake/WinSDK.cmake)

set(CONVERTER_OUT ${VT_ASSET_CONVERTERS_OUT}/HLSL/${CMAKE_GENERATOR_PLATFORM})

vt_add_plugin_target_out_path(ShaderConverterHLSL SHARED "VT_Converters" ${CONVERTER_OUT})

vt_add_third_party_module(ShaderConverterHLSL PRIVATE "DXC" "DXC")

set(DXC_DIR ${VT_ROOT_PATH}/Libs/dxc)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(DXC_BIN_DIR ${DXC_DIR}/Bin/x64)
else()
	set(DXC_BIN_DIR ${DXC_DIR}/Bin/x86)
endif()

set(DXC_BINARY ${DXC_BIN_DIR}/dxcompiler.dll)

add_custom_command(TARGET ShaderConverterHLSL POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${DXC_BINARY} ${CONVERTER_OUT}
)

set(COMPILER_ROOT_PATH ${WIN_SDK_PATH}/bin/${WIN_SDK_VERSION}/${WIN_SDK_PLATFORM})

add_custom_command(TARGET ShaderConverterHLSL POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${COMPILER_ROOT_PATH}/dxil.dll ${CONVERTER_OUT}
)

target_link_libraries(ShaderConverterHLSL
	PUBLIC
		ConverterCommon
	PRIVATE
		GraphicPlatform
		ShaderConverter
)