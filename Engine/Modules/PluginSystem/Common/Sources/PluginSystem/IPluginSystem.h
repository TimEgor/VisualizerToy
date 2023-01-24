#pragma once

#include "IPlugin.h"
#include "Core/FileName/FileName.h"

namespace VT
{
	class IPluginSystem
	{
	public:
		IPluginSystem() = default;
		virtual ~IPluginSystem() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual IPlugin* loadPlugin(const FileName& name) = 0;
		virtual void unloadPlugin(const FileName& name) = 0;
		virtual void unloadPlugin(PluginID id) = 0;

		virtual IPlugin* getPlugin(const FileName& name) = 0;
		virtual IPlugin* getPlugin(PluginID id) = 0;

		virtual PluginID getPluginID(const FileName& name) = 0;
	};
}