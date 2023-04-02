#pragma once

#include "Platform.h"

namespace VT
{
#ifdef VT_WIN_PLATFORM
	inline void outputEnvConsole(const char* msg)
	{
		OutputDebugStringA(msg);
	}

	inline void outputEnvWindow(const char* msg, const char* title)
	{
		MessageBoxA(NULL, msg, title, MB_OK);
	}
#else
	inline void outputEnvConsole(const char* msg) {}
	inline void outputEnvWindow(const char* msg, const char* title) {}
#endif
}

#ifdef VT_WIN_PLATFORM
#define VT_ENV_CONSOLE_OUTPUT(MSG) VT::outputEnvConsole(MSG)
#define VT_WINDOW_OUTPUT(MSG, TITLE) VT::outputEnvWindow(MSG, TITLE)
#else
#define VT_ENV_CONSOLE_OUTPUT(MSG)
#define VT_WINDOW_OUTPUT(MSG, TITLE)
#endif