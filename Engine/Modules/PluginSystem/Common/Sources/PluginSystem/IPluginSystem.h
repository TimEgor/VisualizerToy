#pragma once

#include "IPlugin.h"

namespace VT
{
	class IPluginSystem
	{
	public:
		IPluginSystem() = default;
		virtual ~IPluginSystem() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual bool loadPlugin(const char* name) = 0;
		virtual void unloadPlugin(const char* name) = 0;
		virtual void unloadPlugin(PluginID id) = 0;

		virtual IPlugin* getPlugin(const char* name) = 0;
		virtual IPlugin* getPlugin(PluginID id) = 0;

		virtual PluginID getPluginID(const char* name) = 0;
	};
}