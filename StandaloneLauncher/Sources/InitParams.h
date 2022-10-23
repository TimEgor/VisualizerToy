#pragma once

#include "Core/Platform.h"

#include <string>

namespace VT_Launcher
{
	std::string preparePluginPath(const std::string& pluginName);
	std::string getPlatformPluginPath();
	std::string getGraphicsPluginPath();
}