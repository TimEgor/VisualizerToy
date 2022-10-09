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

		virtual PluginBase* load(const char* name) override;
		virtual void unload(PluginBase* plugin) override;
	};
}
#endif