#include "InitParams.h"

#include "Core/Platform.h"

#include <cassert>

VT::FileName VT_Launcher::preparePluginName(const VT::FileName& pluginName)
{
	return pluginName + '_' + VT::FileName(VT_CONFIG_NAME) + '_'
		+ VT::FileName(VT_PLATFORM_NAME) + VT::FileName(VT_DYNAMIC_LIB_FULL_EXT_NAME);
}

VT::FileName VT_Launcher::preparePluginPath(const VT::FileName& pluginName)
{
	return VT::FileName(VT_ROOT_PATH) + "Plugins/Out/" + preparePluginName(pluginName);
}

VT::FileName VT_Launcher::prepareConverterPath(const VT::FileName& pluginName, const VT::FileName& subdir)
{
	return VT::FileName(VT_ROOT_PATH) + "AssetConverters/Out/" + subdir + '/' + preparePluginName(pluginName);
}

VT::FileName VT_Launcher::getPlatformPluginPath()
{
	VT::FileName pluginName;

#if defined(WIN32)
	pluginName = "Win32PlatformPlugin";
#endif

	return preparePluginPath(pluginName);
}

VT::FileName VT_Launcher::getGraphicsPluginPath()
{
	return preparePluginPath("D3D12GraphicsPlugin");
}

VT::FileName VT_Launcher::getResourceSystemPluginPath()
{
	return preparePluginPath("DummyResourceSystemPlugin");
}

VT::FileName VT_Launcher::getResourceSystemPath()
{
	return VT::FileName(VT_ROOT_PATH) + "DemoProjects/Assets";
}
