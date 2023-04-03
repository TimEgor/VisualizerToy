#include "Win32Platform.h"

#include "Core/UtilitiesMacros.h"

#include "WindowSystem/Win32Window.h"
#include "WindowSystem/Win32WindowEventSystem.h"

#include "InputSystem/Win32InputSystem.h"

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

void VT_WIN32::Win32Platform::collectMonitorInfo()
{
	EnumDisplayMonitors(nullptr, nullptr, [](HMONITOR hMonitor, HDC hdc, LPRECT rect, LPARAM param)
		{
			MONITORINFO monitorInfo = { sizeof(MONITORINFO) };
			if (GetMonitorInfo(hMonitor, &monitorInfo))
			{
				VT::MonitorInfo& info = reinterpret_cast<MonitorInfoCollection*>(param)->emplace_back();

				info.m_resolution.m_x = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
				info.m_resolution.m_y = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

				info.m_basePos.m_x = monitorInfo.rcMonitor.left;
				info.m_basePos.m_y = monitorInfo.rcMonitor.top;
			}

			return TRUE;
		}, reinterpret_cast<LPARAM>(&m_monitorInfos));
}

bool VT_WIN32::Win32Platform::init()
{
	VT_CHECK_INITIALIZATION(initWindowClass());

	collectMonitorInfo();

	return true;
}

VT::IWindow* VT_WIN32::Win32Platform::createWindow()
{
	return new Win32Window();
}

VT::IWindowEventSystem* VT_WIN32::Win32Platform::createWindowEventSystem()
{
	return new Win32WindowEventSystem();
}

VT::IInputSystem* VT_WIN32::Win32Platform::createInputSystem()
{
	return new Win32InputSystem();
}
