#pragma once

#if defined(WIN32) || defined(_WIN32)
#include "PluginSystem/IPluginLoader.h"

namespace VT
{
	class PluginLoaderWin32 final : public IPluginLoader
	{
	private:
		void outputLoadingError(const FileName& name);

	public:
		PluginLoaderWin32() = default;
		virtual ~PluginLoaderWin32() {}

		virtual IPlugin* load(const FileName& name, PluginID id) override;
		virtual void unload(IPlugin* plugin) override;
	};
}
#endif