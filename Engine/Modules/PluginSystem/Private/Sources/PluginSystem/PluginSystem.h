#pragma once

#include "PluginSystem/IPluginSystem.h"
#include "Core/Platform.h"

#include <unordered_map>

namespace VT
{
	class IPluginLoader;

	class PluginSystem final : public IPluginSystem
	{
		using PluginContainer = std::unordered_map<PluginID, IPlugin*>;

	private:
		PluginContainer m_plugins;
		IPluginLoader* m_loader = nullptr;

	public:
		PluginSystem() = default;
		~PluginSystem() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual bool loadPlugin(const FileName& name) override;
		virtual void unloadPlugin(const FileName& name) override;
		virtual void unloadPlugin(PluginID id) override;

		virtual IPlugin* getPlugin(const FileName& name) override;
		virtual IPlugin* getPlugin(PluginID id) override;

		virtual PluginID getPluginID(const FileName& name) override;
	};
}