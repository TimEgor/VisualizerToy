#pragma once

#ifdef WIN32
#include <Windows.h>

#define VT_DLL_EXPORT __declspec(dllexport)
#define VT_DLL_IMPORT __declspec(dllimport)

#define VT_PLATFORM_HANDLE_MODULE HMODULE
#else
#define VT_DLL_EXPORT
#define VT_DLL_IMPORT

#define VT_PLATFORM_HANDLE_MODULE
#endif

namespace VT
{
	constexpr char* getPlatformName()
	{
#ifdef WIN32
        return "Win32";
#elif WIN64
        return "x64";
#else
        return "";
#endif
	}
}

#define VT_PLATFORM_NAME VT::getPlatformName()