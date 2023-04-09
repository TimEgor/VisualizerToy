#include "Win32WindowEventSystem.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "EventSystem/IEventSystem.h"

#include "Win32Window.h"

LRESULT VT_WIN32::Win32WindowEventSystem::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	VT::EngineEnvironment* environment = VT::EngineInstance::getInstance()->getEnvironment();
	VT::IEventSystem* eventSystem = environment->m_eventSystem;
	eventSystem->dispatchEvent(Win32WindowProcedureEvent::getEventType(), Win32WindowProcedureEvent(hwnd, msg, wParam, lParam));

	Win32Window::WindowData* windowData = reinterpret_cast<Win32Window::WindowData*>(GetWindowLongPtr(hwnd, 0));

	if (windowData && windowData->m_header == Win32Window::WindowData::HeaderSignature)
	{
		switch (msg)
		{
		case WM_DESTROY:
			VT::EngineInstance::getInstance()->stop();
			PostQuitMessage(0);
			return 0;

		case WM_ENTERSIZEMOVE:
			VT::EngineInstance::getInstance()->pauseRendering(true);
			break;

		case WM_EXITSIZEMOVE:
		{
			//GlobalEngineInstance::getInstance()->pauseRendering(false);
			//Renderer* renderer = GlobalEngineInstance::getInstance()->getEnvironment()->m_graphicManager->getCurrentRenderer();
			//if (renderer)
			//{
			//	renderer->updateSwapChain(*window);
			//}

			break;
		}
		case WM_SIZE:
		{
			windowData->m_vtWindow->updateSize();
			break;
		}
		}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void VT_WIN32::Win32WindowEventSystem::updateWindowEvents()
{
	MSG msg{ 0 };
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			VT::EngineInstance::getInstance()->stop();
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

bool VT_WIN32::Win32WindowEventSystem::init()
{
	VT::IEventSystem* eventSystem = VT::EngineInstance::getInstance()->getEnvironment()->m_eventSystem;
	return eventSystem->registerEvent(Win32WindowProcedureEvent::getEventType());
}

void VT_WIN32::Win32WindowEventSystem::release()
{
	VT::IEventSystem* eventSystem = VT::EngineInstance::getInstance()->getEnvironment()->m_eventSystem;
	eventSystem->unregisterEvent(Win32WindowProcedureEvent::getEventType());
}
