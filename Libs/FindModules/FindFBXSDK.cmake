set(FBX_SDK_DIR $ENV{FBXSDK_DIR})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.20)
	set(FBX_COMPILER_NAME "vs2019")
	elseif(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.10)
	set(FBX_COMPILER_NAME "vs2017")
	elseif(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19)
	set(FBX_COMPILER_NAME "vs2015")
	elseif(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 18)
	set(FBX_COMPILER_NAME "vs2013")
	elseif(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 17)
	set(FBX_COMPILER_NAME "vs2012")
	elseif(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 16)
	set(FBX_COMPILER_NAME "vs2010")
	else()
	message("Unsupported MSVC compiler version: ${CMAKE_CXX_COMPILER_VERSION}")
	return()
	endif()
else()
	message("Unsupported compiler: ${CMAKE_CXX_COMPILER_ID}")
	return()
endif()  

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(FBX_PLATFORM_NAME "x64")
else()
	set(FBX_PLATFORM_NAME "x86")
endif()

message("FBX SDK path: ${FBX_SDK_DIR}")
message("FBX SDK compiler: ${FBX_COMPILER_NAME}")
message("FBX SDK platform: ${FBX_PLATFORM_NAME}")

set(FBXSDK_LIBRARY_RELEASE_PATH
	${FBX_SDK_DIR}/lib/${FBX_COMPILER_NAME}/${FBX_PLATFORM_NAME}/release/
)
set(FBXSDK_LIBRARY_DEBUG_PATH
	${FBX_SDK_DIR}/lib/${FBX_COMPILER_NAME}/${FBX_PLATFORM_NAME}/debug/
)

include(SelectLibraryConfigurations)
select_library_configurations(FBXSDK)

set(FBXSDK_INCLUDE_DIR ${FBX_SDK_DIR}/include)
set(FBXSDK_LIBRARY ${FBXSDK_LIBRARY_RELEASE_PATH}/libfbxsdk-md.lib)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FBXSDK
	REQUIRED_VARS FBXSDK_LIBRARY FBXSDK_INCLUDE_DIR
)

if(FBXSDK_FOUND)
	set(FBXSDK_INCLUDE_DIRS ${FBXSDK_INCLUDE_DIR})

	if(NOT FBXSDK_LIBRARIES)
		set(FBXSDK_LIBRARIES ${FBXSDK_LIBRARY})
	endif()

	if(NOT TARGET FBXSDK)
		add_library(FBXSDK INTERFACE)

		set_target_properties(FBXSDK PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES ${FBXSDK_INCLUDE_DIR})

		target_link_libraries(FBXSDK
			INTERFACE
			debug
				"${FBXSDK_LIBRARY_DEBUG_PATH}/libfbxsdk-md.lib"
				"${FBXSDK_LIBRARY_DEBUG_PATH}/libxml2-md.lib"
				"${FBXSDK_LIBRARY_DEBUG_PATH}/zlib-md.lib"
			optimized
				"${FBXSDK_LIBRARY_RELEASE_PATH}/libfbxsdk-md.lib"
				"${FBXSDK_LIBRARY_RELEASE_PATH}/libxml2-md.lib"
				"${FBXSDK_LIBRARY_RELEASE_PATH}/zlib-md.lib"
		)
	endif()
endif()