#include "PluginSystem.h"

#include "Core/UtilitiesMacros.h"
#include "PluginLoaders/PluginLoaderCreator.h"

#include <cassert>

bool VT::PluginSystem::init()
{
	m_loader = createPluginLoader();
	VT_CHECK_INITIALIZATION(m_loader);

	return true;
}

void VT::PluginSystem::release()
{
	VT_SAFE_DESTROY(m_loader);
}

VT::IPlugin* VT::PluginSystem::loadPlugin(const FileName& name)
{
	assert(m_loader);

	IPlugin* newPlugin = m_loader->load(name, getPluginID(name));
	if (newPlugin)
	{
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
	assert(m_loader);

	IPlugin* plugin = getPlugin(id);
	assert(plugin);

	plugin->onUnloaded();
	m_loader->unload(plugin);
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
