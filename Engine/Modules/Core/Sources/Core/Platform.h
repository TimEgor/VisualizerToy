#pragma once

#if defined(WIN32) || defined(WIN64)	\
	|| defined(_WIN32) || defined(_WIN64)
#include <Windows.h>

#define VT_DLL_EXPORT __declspec(dllexport)
#define VT_DLL_IMPORT __declspec(dllimport)

#define VT_PLATFORM_HANDLE_MODULE HMODULE

#define VT_DYNAMIC_LIB_EXT_NAME ".dll"
#else
#define VT_DLL_EXPORT
#define VT_DLL_IMPORT

#define VT_PLATFORM_HANDLE_MODULE

#define VT_DYNAMIC_LIB_EXT_NAME ""
#endif

namespace VT
{
	constexpr char* getPlatformName()
	{
#if defined(WIN32) || defined(_WIN32)
		return "Win32";
#elif defined(WIN64) || defined(_WIN64)
		return "x64";
#else
		return "";
#endif
	}
}

#define VT_PLATFORM_NAME VT::getPlatformName()