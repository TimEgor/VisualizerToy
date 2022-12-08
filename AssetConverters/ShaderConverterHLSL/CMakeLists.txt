cmake_minimum_required(VERSION 3.8)

project(ShaderConverterHLSL)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)
include(${VT_ROOT_PATH}/CMake/WinSDK.cmake)

set(CONVERTER_OUT ${VT_ASSET_CONVERTERS_OUT}/HLSL/${CMAKE_GENERATOR_PLATFORM})

vt_add_plugin_target_out_path(ShaderConverterHLSL SHARED "VT_Converters" ${CONVERTER_OUT})

set(COMPILER_ROOT_PATH ${WIN_SDK_PATH}/bin/${WIN_SDK_VERSION}/${WIN_SDK_PLATFORM})
add_custom_command(TARGET ShaderConverterHLSL POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${COMPILER_ROOT_PATH}/dxcompiler.dll ${CONVERTER_OUT}
)
add_custom_command(TARGET ShaderConverterHLSL POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${COMPILER_ROOT_PATH}/dxil.dll ${CONVERTER_OUT}
)

target_link_libraries(ShaderConverterHLSL
	PUBLIC
		ConverterCommon
	PRIVATE
		${WIN_SDK_PATH}/lib/${WIN_SDK_VERSION}/um/${WIN_SDK_PLATFORM}/dxcompiler.lib
		GraphicPlatform
		ShaderConverter
)