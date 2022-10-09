#pragma once

#include "Core/Platform.h"
#include "PluginSystem/PluginBase.h"

namespace VT
{
	constexpr const char* PluginCreatingFunctionName = "VTPluginCreate";
	constexpr const char* PluginReleasingFunctionName = "VTPluginFree";
	using PluginCreatingFunctionPtr = PluginBase*(*)(PluginID, VT_PLATFORM_HANDLE_MODULE);
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