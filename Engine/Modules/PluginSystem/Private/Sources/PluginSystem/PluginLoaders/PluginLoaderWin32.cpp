#ifdef WIN32
#include "PluginLoaderWin32.h"

#include "Core/HashFunctions/CRC32.h"

VT::PluginBase* VT::PluginLoaderWin32::load(const char* name)
{
	PluginID id = crc32(name);

	VT_PLATFORM_HANDLE_MODULE nativeHandle = LoadLibrary(name);
	PluginCreatingFunctionPtr creatorPtr = reinterpret_cast<PluginCreatingFunctionPtr>(GetProcAddress(nativeHandle, PluginCreatingFunctionName));

	return creatorPtr(id, nativeHandle);
}

void VT::PluginLoaderWin32::unload(PluginBase* plugin)
{
	VT_PLATFORM_HANDLE_MODULE nativeHandle = plugin->getNativeHandle();
	PluginReleasingFunctionPtr releasorPtr = reinterpret_cast<PluginReleasingFunctionPtr>(GetProcAddress(nativeHandle, PluginReleasingFunctionName));
	releasorPtr(plugin);

	FreeLibrary(nativeHandle);
}

#endif