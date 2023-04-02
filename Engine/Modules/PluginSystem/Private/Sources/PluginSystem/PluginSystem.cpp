#include "PluginSystem.h"

#include "Core/UtilitiesMacros.h"
#include "PluginLoaders/PluginLoaderCreator.h"

#include <cassert>

void VT::PluginSystem::unloadPluginInternal(IPlugin* plugin)
{
	assert(m_loader);

	plugin->onUnloaded();
	m_loader->unload(plugin);
}

bool VT::PluginSystem::init()
{
	m_loader = createPluginLoader();
	VT_CHECK_INITIALIZATION(m_loader);

	return true;
}

void VT::PluginSystem::release()
{
	for (auto pluginIDIter = m_pluginOrder.rbegin(); pluginIDIter != m_pluginOrder.rend(); ++pluginIDIter)
	{
		unloadPluginInternal(m_plugins[*pluginIDIter]);
	}

	m_plugins = PluginContainer();
	m_pluginOrder = PluginOrderContainer();

	VT_SAFE_DESTROY(m_loader);
}

VT::IPlugin* VT::PluginSystem::loadPlugin(const FileName& name)
{
	PluginID pluginID = getPluginID(name);

	assert(!getPlugin(pluginID));
	assert(m_loader);

	IPlugin* newPlugin = m_loader->load(name, pluginID);
	if (newPlugin)
	{
		m_plugins.insert(std::make_pair(pluginID, newPlugin));
		m_pluginOrder.insert(pluginID);

		newPlugin->onLoaded();
	}

	return newPlugin;
}

void VT::PluginSystem::unloadPlugin(const FileName& name)
{
	unloadPlugin(getPluginID(name));
}

void VT::PluginSystem::unloadPlugin(PluginID id)
{
	IPlugin* plugin = getPlugin(id);
	assert(plugin);

	unloadPluginInternal(plugin);

	m_plugins.erase(id);
	m_pluginOrder.erase(id);
}

VT::IPlugin* VT::PluginSystem::getPlugin(const FileName& name)
{
	return getPlugin(getPluginID(name));
}

VT::IPlugin* VT::PluginSystem::getPlugin(VT::PluginID id)
{
	auto findIter = m_plugins.find(id);
	if (findIter != m_plugins.end())
	{
		return findIter->second;
	}

	return nullptr;
}

VT::PluginID VT::PluginSystem::getPluginID(const FileName& name)
{
	return name.hash();
}
