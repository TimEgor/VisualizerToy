set(D3D12MA_ROOT_PATH ${VT_ROOT_PATH}/Libs/Install/${CMAKE_GENERATOR_PLATFORM}/D3D12MemoryAllocator)

set(D3D12MA_INCLUDE_DIR ${D3D12MA_ROOT_PATH}/include)

set(D3D12MA_LIBRARY_RELEASE ${D3D12MA_ROOT_PATH}/lib/D3D12MA.lib)
set(D3D12MA_LIBRARY_DEBUG ${D3D12MA_ROOT_PATH}/lib/D3D12MAd.lib)

if((EXISTS ${D3D12MA_LIBRARY_RELEASE}) AND (EXISTS ${D3D12MA_LIBRARY_DEBUG}))
	include(SelectLibraryConfigurations)
	select_library_configurations(D3D12MA)
endif()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(D3D12MemoryAllocator DEFAULT_MSG
	D3D12MA_LIBRARY D3D12MA_INCLUDE_DIR)

if(D3D12MemoryAllocator_FOUND)
	if(NOT TARGET D3D12MemoryAllocator)
		add_library(D3D12MemoryAllocator UNKNOWN IMPORTED)
		set_target_properties(D3D12MemoryAllocator PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES "${D3D12MA_INCLUDE_DIR}"
			IMPORTED_LOCATION "${D3D12MA_LIBRARY_RELEASE}"
			IMPORTED_LOCATION_DEBUG "${D3D12MA_LIBRARY_DEBUG}"
		)
	endif()
endif()