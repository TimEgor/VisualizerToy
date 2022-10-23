#include "Win32PlatformPlugin.h"

#include "Win32Platform.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

VT_PLUGIN(Win32PluginPlatform)

void Win32PluginPlatform::onLoaded()
{
	VT::EngineEnvironment* environment = VT::EngineInstance::getInstance()->getEnvironment();
	if (!environment)
	{
		assert(false && "Win32PluginPlatform::onLoaded() : Engine environment is invalid.");
		return;
	}

	if (environment->m_platform)
	{
		assert(false && "Win32PluginPlatform::onLoaded() : Engine environment platform has been already initialized.");
		return;
	}

	environment->m_platform = new Win32Platform(GetModuleHandle(0));
}

void Win32PluginPlatform::onUnloaded()
{

}
