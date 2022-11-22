#pragma once

#ifdef WIN32
#include "PluginSystem/IPluginLoader.h"

namespace VT
{
	class PluginLoaderWin32 final : public IPluginLoader
	{
	public:
		PluginLoaderWin32() = default;
		virtual ~PluginLoaderWin32() {}

		virtual IPlugin* load(const FileName& name, PluginID id) override;
		virtual void unload(IPlugin* plugin) override;
	};
}
#endif