#include "Engine.h"

#include "Core/UtilitiesMacros.h"
#include "Engine/EngineEnvironment.h"

#include "PluginSystem/PluginSystem.h"
#include "WindowSystem/WindowSystem.h"

#include "Platform/IPlatform.h"
#include "GraphicDevice/IGraphicDevice.h"

#include <cassert>

bool VT::Engine::init(const EngineInitParam& initParam)
{

	m_engineEnvironment = new EngineEnvironment();
	VT_CHECK_INITIALIZATION(m_engineEnvironment);

	m_engineEnvironment->m_pluginSystem = new PluginSystem();
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_pluginSystem && m_engineEnvironment->m_pluginSystem->init());

	assert(initParam.m_platformPluginPath);
	m_engineEnvironment->m_pluginSystem->loadPlugin(initParam.m_platformPluginPath);
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_platform && m_engineEnvironment->m_platform->init());

	m_engineEnvironment->m_windowSystem = new WindowSystem();
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_windowSystem
		&& m_engineEnvironment->m_windowSystem->init(
			m_engineEnvironment->m_platform->createWindowContainer(),
			m_engineEnvironment->m_platform->createWindowEventSystem()
		));

	WindowHandle mainWindow = m_engineEnvironment->m_windowSystem->createWindow("VT", { 500, 500 });

	assert(initParam.m_graphicDevicePluginPath);
	m_engineEnvironment->m_pluginSystem->loadPlugin(initParam.m_graphicDevicePluginPath);
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_graphicDevice && m_engineEnvironment->m_graphicDevice->init())

	return true;
}

void VT::Engine::release()
{
	if (m_engineEnvironment)
	{
		VT_SAFE_DESTROY_WITH_RELEASING(m_engineEnvironment->m_graphicDevice);
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