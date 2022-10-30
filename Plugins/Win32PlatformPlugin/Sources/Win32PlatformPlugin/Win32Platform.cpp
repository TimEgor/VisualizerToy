#include "Win32Platform.h"

#include "WindowSystem/Win32Window.h"
#include "WindowSystem/Win32WindowEventSystem.h"
#include "WindowSystem/WindowContainerBase.h"

bool VT_WIN32::Win32Platform::initWindowClass()
{
	WNDCLASS wndClass{};
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = sizeof(Win32Window*);
	wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wndClass.hCursor = LoadCursor(0, IDC_ARROW);
	wndClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	wndClass.hInstance = m_hInstance;
	wndClass.lpfnWndProc = Win32WindowEventSystem::getWndProcPtr();
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

bool VT_WIN32::Win32Platform::init()
{
	VT_CHECK_INITIALIZATION(initWindowClass());

	return true;
}

VT::IWindowContainer* VT_WIN32::Win32Platform::createWindowContainer()
{
	return new VT::WindowContainerBase<Win32Window>();
}

VT::IWindowEventSystem* VT_WIN32::Win32Platform::createWindowEventSystem()
{
	return new Win32WindowEventSystem();
}
