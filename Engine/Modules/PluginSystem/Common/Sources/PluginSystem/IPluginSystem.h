#pragma once

#include "PluginBase.h"

#include <string>

namespace VT
{
	class IPluginSystem
	{
	public:
		IPluginSystem() = default;
		virtual ~IPluginSystem() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual void loadPlugin(const std::string& name) = 0;
		virtual void unloadPlugin(const std::string& name) = 0;
		virtual void unloadPlugin(PluginID id) = 0;

		virtual PluginBase* getPlugin(const std::string& name) = 0;
		virtual PluginBase* getPlugin(PluginID id) = 0;
	};
}