set (ALL_SOURCE_FILE_EXTENSIONS
	"*.h"
	"*.hpp"
	"*.c"
	"*.cpp"
	"*.cmake"
	"*.inl"
)

### vt_get_directory_sources_recurse
function(vt_get_directory_sources_recurse SOURCE_DIR_PATH SOURCE_FILES)
	foreach(FILE_EXTENSION ${ALL_SOURCE_FILE_EXTENSIONS})
		file(GLOB_RECURSE TMP_SRCS "${SOURCE_DIR_PATH}/${FILE_EXTENSION}")
		list(APPEND RESULT_FILES ${TMP_SRCS})
	endforeach(FILE_EXTENSION)

	list(APPEND RESULT_FILES ${${SOURCE_FILES}})
	set(${SOURCE_FILES} ${RESULT_FILES} PARENT_SCOPE)
endfunction()

### vt_get_directory_sources
function(vt_get_directory_sources SOURCE_DIR_PATH SOURCE_FILES)
	foreach(FILE_EXTENSION ${ALL_SOURCE_FILE_EXTENSIONS})
		file(GLOB TMP_SRCS "${SOURCE_DIR_PATH}/${FILE_EXTENSION}")
		list(APPEND RESULT_FILES ${TMP_SRCS})
	endforeach(FILE_EXTENSION)

	list(APPEND RESULT_FILES ${${SOURCE_FILES}})
	set(${SOURCE_FILES} ${RESULT_FILES} PARENT_SCOPE)
endfunction()

### vt_get_subdir_names
function(vt_get_subdirs_names DIR_PATH SUBDIRS_NAMES)
	file(GLOB DIR_FILES RELATIVE ${DIR_PATH} "${DIR_PATH}/*")
	foreach(FILE_NAME ${DIR_FILES})
		if(IS_DIRECTORY "${DIR_PATH}/${FILE_NAME}")
			list(APPEND RESULT_NAMES ${FILE_NAME})
		endif()
	endforeach()

	set(${SUBDIRS_NAMES} ${RESULT_NAMES} PARENT_SCOPE)
endfunction()

### vt_get_all_sources_subdirs
function(vt_get_all_sources_subdirs SOURCE_DIR_PATH SUBDIRS_NAMES SOURCE_FILES)
	set(SUBDIR_GPOUP_SRCS)
	
	vt_get_directory_sources(${SOURCE_DIR_PATH} SUBDIR_GPOUP_SRCS)

	foreach(SUBDIR_NAME ${SUBDIRS_NAMES})
		set(SUBDIR_SRCS)
		vt_get_all_sources_recurse("${SOURCE_DIR_PATH}/${SUBDIR_NAME}" ${SUBDIR_NAME} SUBDIR_SRCS)

		list(APPEND SUBDIR_GPOUP_SRCS ${SUBDIR_SRCS})
	endforeach()

	list(APPEND SUBDIR_GPOUP_SRCS ${${SOURCE_FILES}})
	set(${SOURCE_FILES} ${SUBDIR_GPOUP_SRCS} PARENT_SCOPE)
endfunction()

### vt_get_all_sources_recurse
function(vt_get_all_sources_recurse SOURCE_DIR_PATH GROUP_NAME SOURCE_FILES)
	set(GPOUP_CMAKE_FILE_PATH "${SOURCE_DIR_PATH}/${GROUP_NAME}.cmake")

	if(EXISTS ${GPOUP_CMAKE_FILE_PATH})
		include(${GPOUP_CMAKE_FILE_PATH})
	else()
		vt_get_subdirs_names(${SOURCE_DIR_PATH} SUBDIRS_NAMES)
		vt_get_all_sources_subdirs(${SOURCE_DIR_PATH} "${SUBDIRS_NAMES}" GPOUP_SRCS)
	endif()

	list(APPEND GPOUP_SRCS ${${SOURCE_FILES}})
	set(${SOURCE_FILES} ${GPOUP_SRCS} PARENT_SCOPE)
endfunction()

### vt_generate_source_groups_by_sources_prefix
function(vt_generate_source_groups_by_sources_prefix SOURCE_DIR_PATH PREFIX_DIR SOURCE_FILES)
	string(LENGTH ${SOURCE_DIR_PATH} SOURCE_DIR_PATH_LENGTH)
	foreach(SOURCE_FILE_NAME ${SOURCE_FILES})
		if(${SOURCE_FILE_NAME} MATCHES ${SOURCE_DIR_PATH})
			string(SUBSTRING ${SOURCE_FILE_NAME} ${SOURCE_DIR_PATH_LENGTH} -1 RELATIVE_SOURCE_FILE_PATH)
			string(FIND ${RELATIVE_SOURCE_FILE_PATH} / FILE_NAME_SEPARATOR_POSITION REVERSE)
			string(SUBSTRING ${RELATIVE_SOURCE_FILE_PATH} 0 ${FILE_NAME_SEPARATOR_POSITION} SOURCE_GROUP_NAME)

			if (PREFIX_DIR STREQUAL "")
				source_group(TREE ${SOURCE_DIR_PATH} FILES ${SOURCE_FILE_NAME})
			else()
				source_group(TREE ${SOURCE_DIR_PATH} PREFIX ${PREFIX_DIR} FILES ${SOURCE_FILE_NAME})
			endif()
		endif()
	endforeach(SOURCE_FILE_NAME)
endfunction()

### vt_generate_source_groups_by_sources
function(vt_generate_source_groups_by_sources SOURCE_DIR_PATH SOURCE_FILES)
	vt_generate_source_groups_by_sources_prefix("${SOURCE_DIR_PATH}" "" "${SOURCE_FILES}")
endfunction()