#pragma once

#include "PluginSystem/IPluginSystem.h"
#include "Core/Platform.h"

#include <unordered_map>

namespace VT
{
	class IPluginLoader;

	class PluginSystem final : public IPluginSystem
	{
		using PluginContainer = std::unordered_map<PluginID, PluginBase*>;

	private:
		PluginContainer m_plugins;
		IPluginLoader* m_loader;

	public:
		PluginSystem() = default;
		~PluginSystem() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual bool loadPlugin(const char* name) override;
		virtual void unloadPlugin(const char* name) override;
		virtual void unloadPlugin(PluginID id) override;

		virtual PluginBase* getPlugin(const char* name) override;
		virtual PluginBase* getPlugin(PluginID id) override;

		virtual PluginID getPluginID(const char* name) override;
	};
}