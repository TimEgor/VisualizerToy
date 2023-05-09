#include "Engine.h"

#include "GameSystem/GameSystem.h"
#include "LevelSystem/LevelSystem.h"

#include "Core/UtilitiesMacros.h"
#include "Core/String/Format.h"
#include "Engine/EngineEnvironment.h"

#include "EventSystem/EventSystem.h"

#include "PluginSystem/PluginSystem.h"
#include "Platform/IPlatform.h"
#include "ResourceSystem/IResourceSystem.h"
#include "WindowSystem/WindowSystem.h"

#include "InputSystem/IInputSystem.h"

#include "GraphicPlatform/IGraphicPlatform.h"
#include "GraphicDevice/IGraphicDevice.h"
#include "GraphicResourceManager/NamedGraphicResourceSystem.h"

#include "MeshSystem/MeshSystem.h"
#include "RenderSystem/RenderSystem.h"

#include "DebugUiSystem/DebugUiSystem.h"

#include "Profile/Profile.h"

#include <cassert>

bool VT::Engine::init(const EngineInitParam& initParam)
{

	m_engineEnvironment = new EngineEnvironment();
	VT_CHECK_INITIALIZATION(m_engineEnvironment);

	m_engineEnvironment->m_eventSystem = new EventSystem();
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_eventSystem && m_engineEnvironment->m_eventSystem->init());

	m_engineEnvironment->m_pluginSystem = new PluginSystem();
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_pluginSystem && m_engineEnvironment->m_pluginSystem->init());

	assert(initParam.m_platformPluginPath);
	m_engineEnvironment->m_pluginSystem->loadPlugin(initParam.m_platformPluginPath);
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_platform && m_engineEnvironment->m_platform->init());

	assert(initParam.m_resourceSystenPluginPath);
	m_engineEnvironment->m_pluginSystem->loadPlugin(initParam.m_resourceSystenPluginPath);
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_resourceSystem && m_engineEnvironment->m_resourceSystem->init(initParam.m_resourceSystemPath));

	IWindowEventSystem* m_windowEventSystem = m_engineEnvironment->m_platform->createWindowEventSystem();
	VT_CHECK_INITIALIZATION(m_windowEventSystem && m_windowEventSystem->init());

	m_engineEnvironment->m_windowSystem = new WindowSystem();
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_windowSystem
		&& m_engineEnvironment->m_windowSystem->init(m_windowEventSystem));

	m_engineEnvironment->m_inputSystem = m_engineEnvironment->m_platform->createInputSystem();
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_inputSystem
		&& m_engineEnvironment->m_inputSystem->init());

	assert(initParam.m_graphicDevicePluginPath);
	m_engineEnvironment->m_pluginSystem->loadPlugin(initParam.m_graphicDevicePluginPath);
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_graphicPlatform
		&& m_engineEnvironment->m_graphicPlatform->init(initParam.m_swapChainEnabled));

	m_engineEnvironment->m_graphicDevice = m_engineEnvironment->m_graphicPlatform->createGraphicDevice();
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_graphicDevice
		&& m_engineEnvironment->m_graphicDevice->init(initParam.m_swapChainEnabled));

	m_engineEnvironment->m_graphicDevice->resetContexts();

	for (uint32_t converterIndex = 0; converterIndex < initParam.m_converterPathsCount; ++converterIndex)
	{
		m_engineEnvironment->m_pluginSystem->loadPlugin(initParam.m_converterPaths[converterIndex]);
	}

	m_engineEnvironment->m_graphicResourceManager = new NamedGraphicResourceSystem();
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_graphicResourceManager
		&& m_engineEnvironment->m_graphicResourceManager->init());

	m_engineEnvironment->m_meshSystem = new MeshSystem();
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_meshSystem
		&& m_engineEnvironment->m_meshSystem->init());

	m_engineEnvironment->m_renderSystem = new RenderSystem();
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_renderSystem
		&& m_engineEnvironment->m_renderSystem->init());

	m_engineEnvironment->m_debugUiSystem = new DebugUiSystem();
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_debugUiSystem
		&& m_engineEnvironment->m_debugUiSystem->init());

	// Game systems

	m_engineEnvironment->m_gameSystem = new GameSystem();
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_gameSystem
		&& m_engineEnvironment->m_gameSystem->init());

	m_engineEnvironment->m_levelSystem = new LevelSystem();
	VT_CHECK_INITIALIZATION(m_engineEnvironment->m_levelSystem
		&& m_engineEnvironment->m_levelSystem->init());

	m_engineEnvironment->m_graphicDevice->submitContexts();
	m_engineEnvironment->m_graphicDevice->waitContexts();
	m_engineEnvironment->m_graphicDevice->resetContexts();

	return true;
}

void VT::Engine::release()
{
	if (m_engineEnvironment)
	{
		VT_SAFE_DESTROY_WITH_RELEASING(m_engineEnvironment->m_levelSystem);
		VT_SAFE_DESTROY_WITH_RELEASING(m_engineEnvironment->m_gameSystem);

		VT_SAFE_DESTROY_WITH_RELEASING(m_engineEnvironment->m_debugUiSystem);

		VT_SAFE_DESTROY_WITH_RELEASING(m_engineEnvironment->m_renderSystem);
		VT_SAFE_DESTROY_WITH_RELEASING(m_engineEnvironment->m_meshSystem);

		VT_SAFE_DESTROY_WITH_RELEASING(m_engineEnvironment->m_graphicResourceManager);
		VT_SAFE_DESTROY_WITH_RELEASING(m_engineEnvironment->m_graphicDevice);

		VT_SAFE_DESTROY_WITH_RELEASING(m_engineEnvironment->m_inputSystem);
		VT_SAFE_DESTROY_WITH_RELEASING(m_engineEnvironment->m_windowSystem);
		VT_SAFE_DESTROY_WITH_RELEASING(m_engineEnvironment->m_platform);
		VT_SAFE_DESTROY_WITH_RELEASING(m_engineEnvironment->m_pluginSystem);

		VT_SAFE_DESTROY_WITH_RELEASING(m_engineEnvironment->m_eventSystem);

		VT_SAFE_DESTROY(m_engineEnvironment);
	}
}

VT::IEngine::FrameID VT::Engine::beginFrame()
{
	++m_currentFrameID;

	VT_PROFILE_BEGIN_EVENT(stringFormat("Frame #%d", m_currentFrameID).c_str());

	assert(m_engineEnvironment);

	assert(m_engineEnvironment->m_windowSystem);
	m_engineEnvironment->m_windowSystem->updateWindowEvents();

	m_engineEnvironment->m_inputSystem->update();

	m_currentFrameTime = Clock::getCurrentTime();
	m_deltaTime = Clock::calkTimePointDelta(m_currentFrameTime, m_prevFrameStartTime);

	return m_currentFrameID;
}

void VT::Engine::endFrame()
{
	m_prevFrameStartTime = m_currentFrameTime;
	VT_PROFILE_END_EVENT();
}

void VT::Engine::updateFrame()
{
	assert(m_engineEnvironment);

	assert(m_engineEnvironment->m_gameSystem);
	m_engineEnvironment->m_gameSystem->update(m_deltaTime);

	m_engineEnvironment->m_levelSystem->getCurrentLevel()->getScene()->recalculateTransforms();
}

void VT::Engine::startTimer()
{
	m_startTime = Clock::getCurrentTime();
	m_prevFrameStartTime = m_startTime;
}

void VT::Engine::stop()
{
	m_isStoped = true;
	m_isRenderingPaused = true;
}