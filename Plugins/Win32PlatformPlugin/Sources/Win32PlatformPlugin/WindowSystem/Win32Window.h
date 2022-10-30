#pragma once

#include "Core/Platform.h"
#include "WindowSystem/WindowBase.h"

#define VT_WIN32_WND_CLASS_NAME "VT_Window"

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
	};
}