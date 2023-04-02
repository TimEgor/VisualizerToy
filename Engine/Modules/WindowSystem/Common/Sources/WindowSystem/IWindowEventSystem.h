#pragma once

#include "IWindow.h"
#include "EventSystem/IEvent.h"

namespace VT
{
	class IWindow;

	struct WindowEventBase : public IEvent
	{
		IWindow* m_window = nullptr;

		WindowEventBase(IWindow* window)
			: IEvent(), m_window(window) {}
		virtual ~WindowEventBase() {}
	};

	struct WindowSizeEvent final : public WindowEventBase
	{
		WindowSize m_prevSize = DefaultWindowSize;

		WindowSizeEvent(IWindow* window, const WindowSize& prevSize)
			: WindowEventBase(window), m_prevSize(prevSize) {}

		VT_EVENT_TYPE(WINDOW_SIZE_EVENT);
	};

	class IWindowEventSystem
	{
	public:
		IWindowEventSystem() = default;
		virtual ~IWindowEventSystem() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual void updateWindowEvents() = 0;
	};
}
