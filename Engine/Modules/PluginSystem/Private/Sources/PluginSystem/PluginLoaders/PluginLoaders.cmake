set(LOADER_SRCS
	${SOURCE_DIR_PATH}/PluginLoaderCreator.h
)

if (WIN32)
	set(LOADER_SRCS ${LOADER_SRCS}
		${SOURCE_DIR_PATH}/PluginLoaderWin32.cpp
		${SOURCE_DIR_PATH}/PluginLoaderWin32.h
	)
endif() #WIN32

foreach(LOADER_SRC ${LOADER_SRCS})
	if(EXISTS ${LOADER_SRC})
		list(APPEND GROUP_SRCS ${LOADER_SRC})
	endif()
endforeach()