set(LOADER_SRCS
	${SOURCE_DIR_PATH}/PluginLoader${CMAKE_GENERATOR_PLATFORM}.cpp
	${SOURCE_DIR_PATH}/PluginLoader${CMAKE_GENERATOR_PLATFORM}.h
	${SOURCE_DIR_PATH}/PluginLoaderCreator.h
)

foreach(LOADER_SRC ${LOADER_SRCS})
	if(EXISTS ${LOADER_SRC})
		list(APPEND GROUP_SRCS ${LOADER_SRC})
	endif()
endforeach()