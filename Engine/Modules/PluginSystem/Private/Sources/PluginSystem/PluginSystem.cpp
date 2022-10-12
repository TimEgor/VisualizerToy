#include "PluginSystem.h"

#include "Core/UtilitiesMacros.h"
#include "Core/HashFunctions/CRC32.h"
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

bool VT::PluginSystem::loadPlugin(const char* name)
{
	assert(m_loader);

	PluginBase* newPlugin = m_loader->load(name, getPluginID(name));
	if (newPlugin)
	{
		newPlugin->onLoaded();
	}

	return newPlugin;
}

void VT::PluginSystem::unloadPlugin(const char* name)
{
	unloadPlugin(getPluginID(name));
}

void VT::PluginSystem::unloadPlugin(PluginID id)
{
	assert(m_loader);

	PluginBase* plugin = getPlugin(id);
	assert(plugin);

	plugin->onUnloaded();
	m_loader->unload(plugin);
}

VT::PluginBase* VT::PluginSystem::getPlugin(const char* name)
{
	return getPlugin(getPluginID(name));
}

VT::PluginBase* VT::PluginSystem::getPlugin(VT::PluginID id)
{
	auto findIter = m_plugins.find(id);
	if (findIter != m_plugins.end())
	{
		return findIter->second;
	}

	return nullptr;
}

VT::PluginID VT::PluginSystem::getPluginID(const char* name)
{
	return crc32(name);
}