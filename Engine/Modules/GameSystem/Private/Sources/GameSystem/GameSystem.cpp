#include "GameSystem.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "GameSystem/GameModuleBase.h"
#include "PluginSystem/IPluginSystem.h"

namespace VT
{
	IPluginSystem* getPluginSystem()
	{
		return EngineInstance::getInstance()->getEnvironment()->m_pluginSystem;
	}
}

bool VT::GameSystem::init()
{
	return true;
}

void VT::GameSystem::release()
{
	m_indices = ModuleIndicesCollection();
	m_modules = ModulesCollection();
}

void VT::GameSystem::update(float deltaTime)
{
	for (auto& module : m_modules)
	{
		module->update(deltaTime);
	}
}

VT::IGameModule* VT::GameSystem::loadGameModule(const FileName& modulePath)
{
	GameModuleBase* gameModule = reinterpret_cast<GameModuleBase*>(getPluginSystem()->loadPlugin(modulePath));
	if (gameModule)
	{
		const GameModuleID moduleID = modulePath.hash();

		m_modules.push_back(gameModule);
		m_indices.insert(std::make_pair(moduleID, m_modules.size()));
	}

	return gameModule;
}

void VT::GameSystem::unloadGameModule(const FileName& modulePath)
{
	unloadGameModule(modulePath.hash());
}

void VT::GameSystem::unloadGameModule(GameModuleID moduleID)
{
	auto findModuleIter = m_indices.find(moduleID);
	if (findModuleIter != m_indices.end())
	{
		getPluginSystem()->unloadPlugin(moduleID);

		const size_t moduleIndex = findModuleIter->second;
		if (moduleIndex != m_modules.size() - 1)
		{
			m_modules[moduleIndex] = m_modules.back();

			const GameModuleID swappingModuleID = m_modules[moduleIndex]->getModuleID();
			m_indices[swappingModuleID] = moduleIndex;
		}

		m_indices.erase(findModuleIter);
	}
}

VT::IGameModule* VT::GameSystem::getGameModule(const FileName& modulePath)
{
	return getGameModule(modulePath.hash());
}

VT::IGameModule* VT::GameSystem::getGameModule(GameModuleID moduleID)
{
	auto findModuleIter = m_indices.find(moduleID);
	if (findModuleIter == m_indices.end())
	{
		return nullptr;
	}

	return m_modules[findModuleIter->second];
}
