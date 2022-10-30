#include "Win32WindowEventSystem.h"


#include "Win32PlatformPlugin/Win32PlatformUtils.h"
#include "Win32Window.h"

LRESULT VT_WIN32::Win32WindowEventSystem::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Win32Window* window = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hwnd, 0));

	switch (msg)
	{
	case WM_DESTROY:
		VT::EngineInstance::getInstance()->stop();
		PostQuitMessage(0);
		return 0;

	case WM_ENTERSIZEMOVE:
		VT::EngineInstance::getInstance()->pauseRendering(true);
		return 0;

	case WM_EXITSIZEMOVE:
	{
		//GlobalEngineInstance::getInstance()->pauseRendering(false);
		//Renderer* renderer = GlobalEngineInstance::getInstance()->getEnvironment()->m_graphicManager->getCurrentRenderer();
		//if (renderer)
		//{
		//	renderer->updateSwapChain(*window);
		//}
		return 0;
	}
	case WM_SIZE:
		window->updateSize();
		return 0;
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