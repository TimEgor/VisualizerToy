#pragma once

#include "Core/FileName/FileName.h"

#include <string>

namespace VT_Launcher
{
	VT::FileName preparePluginName(const VT::FileName& pluginName);
	VT::FileName preparePluginPath(const VT::FileName& pluginName);
	VT::FileName prepareConverterPath(const VT::FileName& pluginName, const VT::FileName& subdir);

	VT::FileName getPlatformPluginPath();
	VT::FileName getGraphicsPluginPath();
	VT::FileName getResourceSystemPluginPath();

	VT::FileName getResourceSystemPath();
}