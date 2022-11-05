#include "WindowSystem.h"

#include "Core/UtilitiesMacros.h"

bool VT::WindowSystem::init(IWindowContainer* container, IWindowEventSystem* eventSystem)
{
	assert(!m_windowContainer);
	assert(!m_eventSystem);

	m_windowContainer = container;
	VT_CHECK_INITIALIZATION(m_windowContainer && m_windowContainer->init());

	m_eventSystem = eventSystem;
	VT_CHECK_INITIALIZATION(m_eventSystem);

	return true;
}

void VT::WindowSystem::release()
{
	VT_SAFE_DESTROY_WITH_RELEASING(m_windowContainer);
	VT_SAFE_DESTROY(m_eventSystem);
}

VT::IWindowContainer::NewWindowInfo VT::WindowSystem::createWindow(const char* title, const VT::WindowSize& size)
{
	assert(m_windowContainer);

	const IWindowContainer::NewWindowInfo newWindowInfo = m_windowContainer->addWindow();
	if (newWindowInfo.m_windowPtr)
	{
		newWindowInfo.m_windowPtr->init(title, size);
	}

	return newWindowInfo;
}

void VT::WindowSystem::destroyWindow(WindowHandle handle)
{
	assert(m_windowContainer);

	IWindow* window = m_windowContainer->getWindow(handle);
	if (window)
	{
		window->release();
		m_windowContainer->removeWindow(handle);
	}
}

bool VT::WindowSystem::isValidWindowHandle(WindowHandle handle) const
{
	assert(m_windowContainer);
	return m_windowContainer->isValidWindowHandle(handle);
}

void VT::WindowSystem::updateWindowEvents()
{
	assert(m_eventSystem);
	return m_eventSystem->updateWindowEvents();
}
