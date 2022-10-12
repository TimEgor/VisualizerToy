#pragma once

#include "Core/Platform.h"
#include "PluginSystem/PluginBase.h"
#include "PluginSystem/ExternalPluginCore.h"

#define VT_PLUGIN_FUNC_NAME_STR(NAME) #NAME
#define VT_PLUGIN_FUNC_NAME(NAME) VT_PLUGIN_FUNC_NAME_STR(NAME)

#define VT_PLUGIN_CREATE_FUNC_NAME VT_PLUGIN_FUNC_NAME(VT_PLUGIN_CREATE_FUNC)
#define VT_PLUGIN_RELEASE_FUNC_NAME VT_PLUGIN_FUNC_NAME(VT_PLUGIN_RELEASE_FUNC)

namespace VT
{
	using PluginCreatingFunctionPtr = PluginBase*(*)(PluginID, VT_PLATFORM_HANDLE_MODULE, IEngine*);
	using PluginReleasingFunctionPtr = void(*)(PluginBase*);

	class IPluginLoader
	{
	public:
		IPluginLoader() = default;
		virtual ~IPluginLoader() {}

		virtual PluginBase* load(const char* name) = 0;
		virtual void unload(PluginBase* plugin) = 0;
	};
}