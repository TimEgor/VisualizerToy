#ifdef WIN32
#include "PluginLoaderWin32.h"

#include "Core/HashFunctions/CRC32.h"

VT::PluginBase* VT::PluginLoaderWin32::load(const char* name)
{
	PluginID id = crc32(name);

	VT_PLATFORM_HANDLE_MODULE nativeHandle = LoadLibrary(name);
	PluginCreatingFunctionPtr creatorPtr = reinterpret_cast<PluginCreatingFunctionPtr>(GetProcAddress(nativeHandle, VT_PLUGIN_CREATE_FUNC_NAME));

	return creatorPtr(id, nativeHandle, EngineInstance::getInstance().getEngine());
}

void VT::PluginLoaderWin32::unload(PluginBase* plugin)
{
	VT_PLATFORM_HANDLE_MODULE nativeHandle = plugin->getNativeHandle();
	PluginReleasingFunctionPtr releasorPtr = reinterpret_cast<PluginReleasingFunctionPtr>(GetProcAddress(nativeHandle, VT_PLUGIN_RELEASE_FUNC_NAME));
	releasorPtr(plugin);

	FreeLibrary(nativeHandle);
}

#endif