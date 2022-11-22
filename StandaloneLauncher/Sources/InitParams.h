#pragma once

#include "Core/FileName/FileName.h"

#include <string>

namespace VT_Launcher
{
	void preparePluginPath(const VT::FileName& pluginName, VT::FileName& outPath);
	void getPlatformPluginPath(VT::FileName& outPath);
	void getGraphicsPluginPath(VT::FileName& outPath);
	void getResourceSystemPluginPath(VT::FileName& outPath);
}