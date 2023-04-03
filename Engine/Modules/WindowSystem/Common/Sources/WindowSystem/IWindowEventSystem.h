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

	struct WindowFocusEvent final : public WindowEventBase
	{
		WindowFocusEvent(IWindow* window)
			: WindowEventBase(window) {}

		VT_EVENT_TYPE(WINDOW_FOCUS_EVENT);
	};

	struct WindowUnfocusEvent final : public WindowEventBase
	{
		WindowUnfocusEvent(IWindow* window)
			: WindowEventBase(window) {}

		VT_EVENT_TYPE(WINDOW_UNFOCUS_EVENT);
	};

	struct WindowKeyDownEvent final : public WindowEventBase
	{
		WindowKeyDownEvent(IWindow* window)
			: WindowEventBase(window) {}

		VT_EVENT_TYPE(WINDOW_KEY_DOWN_EVENT);
	};

	struct WindowKeyUpEvent final : public WindowEventBase
	{
		WindowKeyUpEvent(IWindow* window)
			: WindowEventBase(window) {}

		VT_EVENT_TYPE(WINDOW_KEY_UP_EVENT);
	};

	struct WindowMouseButtonDownEvent final : public WindowEventBase
	{
		WindowMouseButtonDownEvent(IWindow* window)
			: WindowEventBase(window) {}

		VT_EVENT_TYPE(WINDOW_MOUSE_BUTTON_DOWN_EVENT);
	};

	struct WindowMouseButtonUpEvent final : public WindowEventBase
	{
		WindowMouseButtonUpEvent(IWindow* window)
			: WindowEventBase(window) {}

		VT_EVENT_TYPE(WINDOW_MOUSE_BUTTON_UP_EVENT);
	};

	struct WindowMouseScrollEvent final : public WindowEventBase
	{
		WindowMouseScrollEvent(IWindow* window)
			: WindowEventBase(window) {}

		VT_EVENT_TYPE(WINDOW_MOUSE_SCROLL_EVENT);
	};

	struct WindowCursorMoveEvent final : public WindowEventBase
	{
		WindowCursorMoveEvent(IWindow* window)
			: WindowEventBase(window) {}

		VT_EVENT_TYPE(WINDOW_CURSOR_MOVE_EVENT);
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
