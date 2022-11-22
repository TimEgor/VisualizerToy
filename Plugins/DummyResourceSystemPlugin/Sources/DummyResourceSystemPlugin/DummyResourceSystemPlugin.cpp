#include "DummyResourceSystemPlugin.h"

#include "DummyResourceSystem.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

VT_PLUGIN(VT_DUMMY_RS::DummyResourceSystemPlugin)

namespace VT_DUMMY_RS
{
	void DummyResourceSystemPlugin::onLoaded()
	{
		VT::EngineEnvironment* environment = VT::EngineInstance::getInstance()->getEnvironment();
		if (!environment)
		{
			assert(false && "DummyResourceSystemPlugin::onLoaded() : Engine environment is invalid.");
			return;
		}

		if (environment->m_resourceSystem)
		{
			assert(false && "DummyResourceSystemPlugin::onLoaded() : Engine environment resourceSystem has been already initialized.");
			return;
		}

		environment->m_resourceSystem = new DummyResourceSystem();
	}

	void DummyResourceSystemPlugin::onUnloaded()
	{
		VT::EngineEnvironment* environment = VT::EngineInstance::getInstance()->getEnvironment();
		if (!environment)
		{
			assert(false && "DummyResourceSystemPlugin::onUnloaded() : Engine environment is invalid.");
			return;
		}

		VT::IResourceSystem* resourceSystem = environment->m_resourceSystem;

		if (!resourceSystem)
		{
			assert(false && "DummyResourceSystemPlugin::onUnloaded() : Engine environment resourceSystem is null.");
			return;
		}

		if (resourceSystem->getType() != DummyResourceSystem::getResourceSystemType())
		{
			assert(false && "DummyResourceSystemPlugin::onUnloaded() : Engine environment resourceSystem isn't Dummmy.");
			return;
		}

		VT_SAFE_DESTROY_WITH_RELEASING(resourceSystem);
	}
}