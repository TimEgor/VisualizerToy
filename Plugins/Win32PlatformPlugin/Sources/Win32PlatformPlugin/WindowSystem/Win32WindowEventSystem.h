#pragma once

#include "Core/Platform.h"
#include "WindowSystem/IWindowEventSystem.h"

namespace VT_WIN32
{
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