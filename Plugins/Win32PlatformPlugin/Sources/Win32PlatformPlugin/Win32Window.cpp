#include "Win32Window.h"

#include "Win32PlatformUtils.h"

bool Win32Window::init(const char* title, const VT::WindowSize& size)
{
	Win32Platform* win32Platform = getWin32EnvironmentPlatform();

	m_windowSize = size;

	m_hWnd = CreateWindow(VT_WIN32_WND_CLASS_NAME, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		m_windowSize.m_width, m_windowSize.m_height, NULL, NULL, win32Platform->getHInstance(), NULL);

	if (!m_hWnd)
	{
		return false;
	}

	SetWindowLongPtr(m_hWnd, 0, reinterpret_cast<LONG_PTR>(this));

	ShowCursor(true);
	
	show();
	UpdateWindow(m_hWnd);

    return true;
}

void Win32Window::release()
{
	if (m_hWnd)
	{
		DestroyWindow(m_hWnd);
	}
}

void Win32Window::show()
{
	ShowWindow(m_hWnd, SW_NORMAL);
}

void Win32Window::hide()
{
	ShowWindow(m_hWnd, SW_HIDE);
}

void Win32Window::updateSize()
{
	RECT clientSize;
	GetClientRect(m_hWnd, &clientSize);
	m_clientAreaSize.m_width = static_cast<VT::WindowSize::ValueType>(clientSize.right - clientSize.left);
	m_clientAreaSize.m_height = static_cast<VT::WindowSize::ValueType>(clientSize.bottom - clientSize.top);

	m_cutoutsArea.m_top = static_cast<VT::WindowArea::ValueType>(clientSize.top);
	m_cutoutsArea.m_bottom = static_cast<VT::WindowArea::ValueType>(m_windowSize.m_height - clientSize.bottom);
	m_cutoutsArea.m_right = static_cast<VT::WindowArea::ValueType>(m_windowSize.m_width - clientSize.right);
	m_cutoutsArea.m_left = static_cast<VT::WindowArea::ValueType>(clientSize.left);
}
