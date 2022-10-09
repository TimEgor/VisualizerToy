#pragma once

#ifdef WIN32
#include "PluginLoaderWin32.h"
#endif

namespace VT
{
	inline IPluginLoader* createPluginLoader()
	{
#ifdef WIN32
		return new PluginLoaderWin32();
#endif
	}
}