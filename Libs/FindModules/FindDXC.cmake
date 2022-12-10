set(DXC_DIR ${VT_ROOT_PATH}/Libs/dxc)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(DXC_BIN_DIR ${DXC_DIR}/Bin/x64)
	set(DXC_LIB_DIR ${DXC_DIR}/Lib/x64)
else()
	set(DXC_BIN_DIR ${DXC_DIR}/Bin/x86)
	set(DXC_LIB_DIR ${DXC_DIR}/Lib/x86)
endif()

include(SelectLibraryConfigurations)
select_library_configurations(DXC)

set(DXC_INCLUDE ${DXC_DIR}/Include)
set(DXC_BINARY ${DXC_BIN_DIR}/dxcompiler.dll)
set(DXC_LIBRARY ${DXC_LIB_DIR}/dxcompiler.lib)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DXC
	REQUIRED_VARS DXC_INCLUDE DXC_BINARY DXC_LIBRARY
)

if(DXC_FOUND)
	if(NOT TARGET DXC)
		add_library(DXC INTERFACE)

		set_target_properties(DXC PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES ${DXC_INCLUDE})

		target_link_libraries(DXC
			INTERFACE ${DXC_LIBRARY}
		)
	endif()
endif()