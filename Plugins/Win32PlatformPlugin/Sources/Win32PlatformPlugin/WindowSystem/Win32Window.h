#pragma once

#include "Core/Platform.h"
#include "WindowSystem/WindowBase.h"

#define VT_WIN32_WND_CLASS_NAME "VT_Window"
#define VT_WINDOW_WIN32_TYPE

namespace VT_WIN32
{
	class Win32Window final : public VT::WindowBase
	{
	private:
		HWND m_hWnd = 0;

	public:
		Win32Window() = default;

		virtual bool init(const char* title, const VT::WindowSize& size) override;
		virtual void release() override;

		virtual void show() override;
		virtual void hide() override;

		void updateSize();

		virtual void* getNativeHandle() const override { return m_hWnd; }

		VT_WINDOW_TYPE(VT_WINDOW_WIN32_TYPE);
	};
}