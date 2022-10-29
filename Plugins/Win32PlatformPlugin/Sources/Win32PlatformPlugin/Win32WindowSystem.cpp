#include "Win32WindowSystem.h"

#include "Win32PlatformUtils.h"

LRESULT VT_WIN32::Win32WindowSystem::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

bool VT_WIN32::Win32WindowSystem::init()
{
	VT_CHECK_INITIALIZATION(registerClass());
	VT_CHECK_INITIALIZATION(initWindowPool());

	return true;
}

void VT_WIN32::Win32WindowSystem::release()
{
	releaseWindowPool();
}

void VT_WIN32::Win32WindowSystem::updateWindowEvents()
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

bool VT_WIN32::Win32WindowSystem::registerClass()
{
	Win32Platform* platform = getWin32EnvironmentPlatform();

	WNDCLASS wndClass{};
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = sizeof(Win32Window*);
	wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wndClass.hCursor = LoadCursor(0, IDC_ARROW);
	wndClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	wndClass.hInstance = platform->getHInstance();
	wndClass.lpfnWndProc = wndProc;
	wndClass.lpszClassName = VT_WIN32_WND_CLASS_NAME;
	wndClass.lpszMenuName = 0;
	wndClass.style = 0;

	if (!RegisterClass(&wndClass))
	{
		assert(false && "Win32WindowSystem::registerClass() : WNDCLASS registration failed.");
		return false;
	}

	return true;
}