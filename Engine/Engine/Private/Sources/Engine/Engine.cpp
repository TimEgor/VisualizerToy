#include "Engine.h"

#include "Core/UtilitiesMacros.h"
#include "Engine/EngineEnvironment.h"

#include "PluginSystem/PluginSystem.h"
#include "WindowSystem/WindowSystem.h"

#include "Platform/IPlatform.h"
#include "GraphicPlatform/IGraphicPlatform.h"
#include "GraphicDevice/IGraphicDevice.h"
#include "GraphicResourceManager/GraphicResourceManager.h"

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

	assert(initParam.m_graphicDevicePluginPath);
	m_engineEnvironment->m_pluginSystem->loadPlugin(initParam.m_graphicDevicePluginPath);
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_graphicPlatform
		&& m_engineEnvironment->m_graphicPlatform->init(initParam.m_swapChainEnabled));

	m_engineEnvironment->m_graphicDevice = m_engineEnvironment->m_graphicPlatform->createGraphicDevice();
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_graphicDevice
		&& m_engineEnvironment->m_graphicDevice->init(initParam.m_swapChainEnabled));

	m_engineEnvironment->m_graphicResourceManager = new GraphicResourceManager();
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_graphicResourceManager
		&& m_engineEnvironment->m_graphicResourceManager->init(
			ResourceManagerContainerCollection{
				m_engineEnvironment->m_graphicPlatform->createSwapChainResourceContainer(),
				m_engineEnvironment->m_graphicPlatform->createTexture2DResourceContainer()
			},
			initParam.m_swapChainEnabled
		));

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