#pragma once

#define VT_TO_STRING(S) #S
#define VT_TO_STRING_EXPR(EXPR) VT_TO_STRING(EXPR)
#define VT_TO_CHAR(C) #@C

#define VT_CONCAT(A, B) A##B

#ifdef WIN32
#define VT_FUNCTION_NAME	__FUNCTION__
#define VT_LINE				__LINE__
#define VT_COUNTER			__COUNTER__
#else
#define VT_FUNCTION_NAME	__func__
#define VT_LINE				__line__
#define VT_COUNTER			__counter__
#endif