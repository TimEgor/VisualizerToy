#include "Engine.h"

#include "Core/UtilitiesMacros.h"
#include "Engine/EngineEnvironment.h"
#include "PluginSystem/PluginSystem.h"

bool VT::Engine::init()
{
	m_engineEnvironment = new EngineEnvironment();
	VT_CHECK_INITIALIZATION(m_engineEnvironment);

	m_engineEnvironment->m_pluginSystem = new PluginSystem();
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_pluginSystem && m_engineEnvironment->m_pluginSystem->init());

	return true;
}

void VT::Engine::release()
{
	if (m_engineEnvironment)
	{
		VT_SAFE_DESTROY_WITH_RELEASING(m_engineEnvironment->m_pluginSystem);

		delete m_engineEnvironment;
		m_engineEnvironment = nullptr;
	}
}

void VT::Engine::run()
{}

void VT::Engine::updateFrame()
{}

void VT::Engine::startTimer()
{}

void VT::Engine::stop()
{}

bool VT::Engine::isStoped() const
{
	return false;
}

void VT::Engine::pauseRendering(bool state)
{}

bool VT::Engine::isRenderingPaused() const
{
	return false;
}