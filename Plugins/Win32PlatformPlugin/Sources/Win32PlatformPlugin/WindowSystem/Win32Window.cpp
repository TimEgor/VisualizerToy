#include "Win32Window.h"

#include "Core/String/Format.h"
#include "Win32PlatformPlugin/Win32PlatformUtils.h"

bool VT_WIN32::Win32Window::init(const char* title, const VT::WindowSize& size)
{
	Win32Platform* win32Platform = getWin32EnvironmentPlatform();

	m_windowSize = size;

	m_hWnd = CreateWindow(VT_WIN32_WND_CLASS_NAME, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		m_windowSize.m_x, m_windowSize.m_y, NULL, NULL, win32Platform->getHInstance(), NULL);

	if (!m_hWnd)
	{
		return false;
	}

	SetWindowLongPtr(m_hWnd, 0, reinterpret_cast<LONG_PTR>(&m_windowData));

	ShowCursor(true);
	
	show();
	UpdateWindow(m_hWnd);

	VT_CHECK_INITIALIZATION(initEvents());

    return true;
}

void VT_WIN32::Win32Window::release()
{
	releaseEvents();

	if (m_hWnd)
	{
		DestroyWindow(m_hWnd);
	}
}

void VT_WIN32::Win32Window::show()
{
	ShowWindow(m_hWnd, SW_NORMAL);
}

void VT_WIN32::Win32Window::hide()
{
	ShowWindow(m_hWnd, SW_HIDE);
}

void VT_WIN32::Win32Window::updateSize()
{
	RECT clientSize;
	GetClientRect(m_hWnd, &clientSize);
	m_clientAreaSize.m_x = static_cast<uint16_t>(clientSize.right - clientSize.left);
	m_clientAreaSize.m_y = static_cast<uint16_t>(clientSize.bottom - clientSize.top);

	m_cutoutsArea.m_top = static_cast<VT::WindowArea::ValueType>(clientSize.top);
	m_cutoutsArea.m_bottom = static_cast<VT::WindowArea::ValueType>(m_windowSize.m_y - clientSize.bottom);
	m_cutoutsArea.m_right = static_cast<VT::WindowArea::ValueType>(m_windowSize.m_x - clientSize.right);
	m_cutoutsArea.m_left = static_cast<VT::WindowArea::ValueType>(clientSize.left);
}
