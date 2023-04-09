vt_get_subdirs_names(${SOURCE_DIR_PATH} SUBDIRS_NAMES)
vt_get_all_sources_subdirs(${SOURCE_DIR_PATH} "${SUBDIRS_NAMES}" GROUP_SRCS)

set(IMGUI_ROOT_PATH ${VT_ROOT_PATH}/Libs/imgui)
set(IMGUI_INCLUDING_FILES
	"imgui*.cpp"
	"imgui*.h"
)

set(IMGUI_CORE_SRCS)

foreach(IMGUI_FILES ${IMGUI_INCLUDING_FILES})
	file(GLOB TMP_SRCS "${IMGUI_ROOT_PATH}/${IMGUI_FILES}")
	list(APPEND IMGUI_CORE_SRCS ${TMP_SRCS})
endforeach(IMGUI_FILES)

list(APPEND IMGUI_CORE_SRCS ${IMGUI_ROOT_PATH}/backends/imgui_impl_dx12.cpp)
list(APPEND IMGUI_CORE_SRCS ${IMGUI_ROOT_PATH}/backends/imgui_impl_dx12.h)

list(APPEND GROUP_SRCS ${IMGUI_CORE_SRCS})