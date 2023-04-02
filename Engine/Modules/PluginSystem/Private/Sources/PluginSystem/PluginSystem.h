#pragma once

#include "PluginSystem/IPluginSystem.h"

#include <unordered_map>
#include <set>

namespace VT
{
	class IPluginLoader;

	class PluginSystem final : public IPluginSystem
	{
		using PluginContainer = std::unordered_map<PluginID, IPlugin*>;
		using PluginOrderContainer = std::set<PluginID>;

	private:
		PluginContainer m_plugins;
		PluginOrderContainer m_pluginOrder;

		IPluginLoader* m_loader = nullptr;

		void unloadPluginInternal(IPlugin* plugin);

	public:
		PluginSystem() = default;
		~PluginSystem() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual IPlugin* loadPlugin(const FileName& name) override;
		virtual void unloadPlugin(const FileName& name) override;
		virtual void unloadPlugin(PluginID id) override;

		virtual IPlugin* getPlugin(const FileName& name) override;
		virtual IPlugin* getPlugin(PluginID id) override;

		virtual PluginID getPluginID(const FileName& name) override;
	};
}