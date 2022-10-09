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

void VT::PluginSystem::loadPlugin(const std::string& name)
{
	assert(m_loader);

	PluginBase* newPlugin = m_loader->load(name.c_str());
	newPlugin->onLoaded();
}

void VT::PluginSystem::unloadPlugin(const std::string& name)
{
	unloadPlugin(crc32(name.c_str()));
}

void VT::PluginSystem::unloadPlugin(PluginID id)
{
	assert(m_loader);

	PluginBase* plugin = getPlugin(id);
	assert(plugin);

	plugin->onUnloaded();
	m_loader->unload(plugin);
}

VT::PluginBase* VT::PluginSystem::getPlugin(const std::string& name)
{
	return getPlugin(crc32(name.c_str()));
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