#pragma once

#include "Core/Platform.h"
#include "WindowSystem/IWindowEventSystem.h"
#include "EventSystem/IEventSystem.h"

namespace VT_WIN32
{
	struct Win32WindowProcedureEvent final : public VT::IEvent
	{
		HWND m_hwnd;
		UINT m_msg;
		WPARAM m_wParam;
		LPARAM m_lParam;

		Win32WindowProcedureEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
			: m_hwnd(hwnd), m_msg(msg), m_wParam(wParam), m_lParam(lParam) {}

		VT_EVENT_TYPE(WIN32_WINDOW_PROCEDURE_EVENT_TYPE);
	};

	class Win32WindowEventSystem final : public VT::IWindowEventSystem
	{
	private:
		static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	public:
		Win32WindowEventSystem() = default;
		~Win32WindowEventSystem() { release(); }

		virtual void updateWindowEvents() override;

		virtual bool init() override;
		virtual void release() override;

		static WNDPROC getWndProcPtr() { return wndProc; }
	};
}