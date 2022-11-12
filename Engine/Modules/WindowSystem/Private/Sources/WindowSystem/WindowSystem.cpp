#include "WindowSystem.h"

#include "Core/UtilitiesMacros.h"

#include "Engine/IEngine.h"
#include "Engine/EngineInstance.h"
#include "Engine/EngineEnvironment.h"
#include "Platform/IPlatform.h"

bool VT::WindowSystem::init(IWindowEventSystem* eventSystem)
{
	assert(!m_eventSystem);

	m_eventSystem = eventSystem;
	VT_CHECK_INITIALIZATION(m_eventSystem);

	return true;
}

void VT::WindowSystem::release()
{
	VT_SAFE_DESTROY(m_eventSystem);
}

VT::IWindow* VT::WindowSystem::createWindow(const char* title, const VT::WindowSize& size)
{
	IWindow* newWindow = EngineInstance::getInstance().getEngine()->getEnvironment()->m_platform->createWindow();
	if (newWindow)
	{
		newWindow->init(title, size);
	}

	return newWindow;
}

void VT::WindowSystem::destroyWindow(IWindow* window)
{
	VT_SAFE_DESTROY_WITH_RELEASING(window);
}

void VT::WindowSystem::updateWindowEvents()
{
	assert(m_eventSystem);
	return m_eventSystem->updateWindowEvents();
}
