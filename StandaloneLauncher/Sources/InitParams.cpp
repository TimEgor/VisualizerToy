#include "InitParams.h"

#include <cassert>

std::string VT_Launcher::preparePluginPath(const std::string& pluginName)
{
	return std::string(VT_ROOT_PATH) + "Plugins/Out/" + pluginName + '_'
		+ VT_CONFIG_NAME + '_' + VT_PLATFORM_NAME + VT_DYNAMIC_LIB_EXT_NAME;
}

std::string VT_Launcher::getPlatformPluginPath()
{
	std::string pluginName;
#if defined(WIN32) || defined(WIN64)
	pluginName = "Win32PlatformPlugin";
#endif
	assert(!pluginName.empty());

	return preparePluginPath(pluginName);
}

std::string VT_Launcher::getGraphicsPluginPath()
{
	std::string pluginName = "VulkanGraphicsPlugin";

	return preparePluginPath(pluginName);
}
