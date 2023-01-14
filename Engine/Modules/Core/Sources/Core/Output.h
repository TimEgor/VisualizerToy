#pragma once

#include "Platform.h"

namespace VT
{
#ifdef VT_WIN_PLATFORM
	void outputEnvConsoleWin(const char* msg)
	{
		OutputDebugStringA(msg);
	}

	void outputEnvWindowWin(const char* msg, const char* title)
	{
		MessageBoxA(NULL, msg, title, MB_OK);
	}
#endif
}

#ifdef VT_WIN_PLATFORM
#define VT_ENV_CONSOLE_OUTPUT(MSG) VT::outputEnvConsoleWin(MSG)
#define VT_WINDOW_OUTPUT(MSG, TITLE) VT::outputEnvWindowWin(MSG, TITLE)
#else
#define VT_ENV_CONSOLE_OUTPUT(STR)
#define VT_WINDOW_OUTPUT(MSG, TITLE)
#endif