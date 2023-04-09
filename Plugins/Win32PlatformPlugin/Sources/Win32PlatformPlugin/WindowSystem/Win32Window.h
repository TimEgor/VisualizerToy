#pragma once

#include "Core/Platform.h"
#include "WindowSystem/WindowBase.h"

#define VT_WIN32_WND_CLASS_NAME "VT_Window"
#define VT_WINDOW_WIN32_TYPE

namespace VT_WIN32
{
	class Win32Window final : public VT::WindowBase
	{
	public:
		struct WindowData final
		{
			inline static constexpr uint32_t HeaderSignature = VT::crc32("VT_WINDOW_SIGNATURE");

			uint32_t m_header = HeaderSignature;
			Win32Window* m_vtWindow = nullptr;

			WindowData(Win32Window* window) : m_vtWindow(window) {}
		};

	private:
		WindowData m_windowData;
		HWND m_hWnd = 0;

	public:
		Win32Window() : m_windowData(this) {}
		~Win32Window() { release(); }

		virtual bool init(const char* title, const VT::WindowSize& size) override;
		virtual void release() override;

		virtual void show() override;
		virtual void hide() override;

		void updateSize();

		virtual void* getNativeHandle() const override { return m_hWnd; }

		VT_WINDOW_TYPE(VT_WINDOW_WIN32_TYPE);
	};
}