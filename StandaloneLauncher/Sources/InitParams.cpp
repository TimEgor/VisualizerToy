#include "InitParams.h"

#include "Core/Platform.h"

#include <cassert>

void VT_Launcher::preparePluginPath(const VT::FileName& pluginName, VT::FileName& outPath)
{
	outPath = VT::FileName(VT_ROOT_PATH) + "Plugins/Out/" + pluginName + '_'
		+ VT::FileName(VT_CONFIG_NAME) + '_' + VT::FileName(VT_PLATFORM_NAME) + VT::FileName(VT_DYNAMIC_LIB_EXT_NAME);
}

void VT_Launcher::getPlatformPluginPath(VT::FileName& outPath)
{
	VT::FileName pluginName;
#if defined(WIN32) || defined(WIN64)
	pluginName = "Win32PlatformPlugin";
#endif
	assert(!pluginName.empty());

	preparePluginPath(pluginName, outPath);
}

void VT_Launcher::getGraphicsPluginPath(VT::FileName& outPath)
{
	preparePluginPath("VulkanGraphicsPlugin", outPath);
}

void VT_Launcher::getResourceSystemPluginPath(VT::FileName& outPath)
{
	preparePluginPath("DummyResourceSystemPlugin", outPath);
}
