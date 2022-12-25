#include "Engine.h"

#include "Core/UtilitiesMacros.h"
#include "Engine/EngineEnvironment.h"

#include "PluginSystem/PluginSystem.h"
#include "WindowSystem/WindowSystem.h"

#include "Platform/IPlatform.h"
#include "ResourceSystem/IResourceSystem.h"
#include "GraphicPlatform/IGraphicPlatform.h"
#include "GraphicDevice/IGraphicDevice.h"
#include "GraphicResourceManager/NamedGraphicResourceSystem.h"
#include "RenderSystem/RenderSystem.h"

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

	assert(initParam.m_resourceSystenPluginPath);
	m_engineEnvironment->m_pluginSystem->loadPlugin(initParam.m_resourceSystenPluginPath);
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_resourceSystem && m_engineEnvironment->m_resourceSystem->init());

	m_engineEnvironment->m_windowSystem = new WindowSystem();
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_windowSystem
		&& m_engineEnvironment->m_windowSystem->init(m_engineEnvironment->m_platform->createWindowEventSystem()));

	assert(initParam.m_graphicDevicePluginPath);
	m_engineEnvironment->m_pluginSystem->loadPlugin(initParam.m_graphicDevicePluginPath);
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_graphicPlatform
		&& m_engineEnvironment->m_graphicPlatform->init(initParam.m_swapChainEnabled));

	m_engineEnvironment->m_graphicDevice = m_engineEnvironment->m_graphicPlatform->createGraphicDevice();
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_graphicDevice
		&& m_engineEnvironment->m_graphicDevice->init(initParam.m_swapChainEnabled));

	assert(initParam.m_shaderConverterPath);
	m_engineEnvironment->m_pluginSystem->loadPlugin(initParam.m_shaderConverterPath);

	m_engineEnvironment->m_graphicResourceManager = new NamedGraphicResourceSystem();
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_graphicResourceManager
		&& m_engineEnvironment->m_graphicResourceManager->init());

	m_engineEnvironment->m_renderSystem = new RenderSystem();
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_renderSystem
		&& m_engineEnvironment->m_renderSystem->init());

	return true;
}

void VT::Engine::release()
{
	if (m_engineEnvironment)
	{
		VT_SAFE_DESTROY_WITH_RELEASING(m_engineEnvironment->m_renderSystem);
		VT_SAFE_DESTROY_WITH_RELEASING(m_engineEnvironment->m_graphicResourceManager);
		VT_SAFE_DESTROY_WITH_RELEASING(m_engineEnvironment->m_graphicDevice);
		VT_SAFE_DESTROY_WITH_RELEASING(m_engineEnvironment->m_windowSystem);
		VT_SAFE_DESTROY_WITH_RELEASING(m_engineEnvironment->m_platform);
		VT_SAFE_DESTROY_WITH_RELEASING(m_engineEnvironment->m_pluginSystem);

		VT_SAFE_DESTROY(m_engineEnvironment);
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