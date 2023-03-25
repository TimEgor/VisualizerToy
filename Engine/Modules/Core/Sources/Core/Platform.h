#pragma once

#include "CommonMacros.h"

#if defined(WIN32) || defined(_WIN32)
#include <Windows.h>

#define VT_WIN_PLATFORM

#define VT_DYNAMIC_LIB_EXPORT __declspec(dllexport)
#define VT_DYNMIC_LIB_IMPORT __declspec(dllimport)

#define VT_PLATFORM_HANDLE_MODULE HMODULE

#define VT_DYNAMIC_LIB_EXT dll

#if defined(WIN64) || defined(_WIN64)
#define VT_PLATFORM x64
#else
#define VT_PLATFORM Win32
#endif

#else
#define VT_DYNAMIC_LIB_EXPORT
#define VT_DYNMIC_LIB_IMPORT

#define VT_PLATFORM_HANDLE_MODULE

#define VT_DYNAMIC_LIB_EXT

#define VT_PLATFORM
#endif

#define VT_DYNAMIC_LIB_EXT_NAME VT_TO_STRING_EXPR(VT_DYNAMIC_LIB_EXT)
#define VT_DYNAMIC_LIB_FULL_EXT_NAME VT_TO_STRING_EXPR(.VT_DYNAMIC_LIB_EXT)

#define VT_PLATFORM_NAME VT_TO_STRING_EXPR(VT_PLATFORM)