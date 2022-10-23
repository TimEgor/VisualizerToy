#include "Engine.h"

#include "Core/UtilitiesMacros.h"
#include "Engine/EngineEnvironment.h"
#include "PluginSystem/PluginSystem.h"

#include "Platform/IPlatform.h"
#include "WindowSystem/IWindowSystem.h"

#include <string>
#include <cassert>

void VT::Engine::initEnvironmentPlatform()
{
	//tmp realization

	m_engineEnvironment->m_pluginSystem->loadPlugin((std::string(VT_ROOT_PATH) + "Plugins/Out/Win32PlatformPlugin_Debug_Win32.dll").c_str());
}

bool VT::Engine::init()
{
	m_engineEnvironment = new EngineEnvironment();
	VT_CHECK_INITIALIZATION(m_engineEnvironment);

	m_engineEnvironment->m_pluginSystem = new PluginSystem();
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_pluginSystem && m_engineEnvironment->m_pluginSystem->init());

	initEnvironmentPlatform();
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_platform);

	m_engineEnvironment->m_windowSystem = m_engineEnvironment->m_platform->createWindowSystem();
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_windowSystem && m_engineEnvironment->m_windowSystem->init());

	WindowHandle mainWindow = m_engineEnvironment->m_windowSystem->createWindow("VT", { 500, 500 });

	return true;
}

void VT::Engine::release()
{
	if (m_engineEnvironment)
	{
		VT_SAFE_DESTROY_WITH_RELEASING(m_engineEnvironment->m_windowSystem);
		VT_SAFE_DESTROY(m_engineEnvironment->m_platform);
		VT_SAFE_DESTROY_WITH_RELEASING(m_engineEnvironment->m_pluginSystem);

		delete m_engineEnvironment;
		m_engineEnvironment = nullptr;
	}
}

void VT::Engine::run()
{
	startTimer();

	while (!m_isStoped)
	{
		updateFrame();
	}
}

void VT::Engine::updateFrame()
{
	assert(m_engineEnvironment);

	assert(m_engineEnvironment->m_windowSystem);
	m_engineEnvironment->m_windowSystem->updateWindowEvents();
}

void VT::Engine::startTimer()
{
	m_startTime = Clock::getCurrentTime();
	m_lastFrameStartTime = m_startTime;
}

void VT::Engine::stop()
{
	m_isStoped = true;
	m_isRenderingPaused = true;
}