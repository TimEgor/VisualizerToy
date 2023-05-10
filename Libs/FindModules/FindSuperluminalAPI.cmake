set(SuperLuminalAPI_FindModule_DIR $ENV{SUPERLUMINAL_API_DIR}/FindSuperluminalAPI.cmake)
if(EXISTS ${SuperLuminalAPI_FindModule_DIR})
	include(${SuperLuminalAPI_FindModule_DIR})
endif()