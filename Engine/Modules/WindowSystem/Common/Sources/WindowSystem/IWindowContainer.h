#pragma once

#include "IWindow.h"

#include <cstdint>

namespace VT
{
	using WindowHandle = uint32_t;
	constexpr WindowHandle InvalidWindowHandle = 0;

	class IWindowContainer
	{
	public:
		struct NewWindowInfo final
		{
			WindowHandle m_windowHandle = InvalidWindowHandle;
			IWindow* m_windowPtr = nullptr;
		};

	public:
		IWindowContainer() = default;
		virtual ~IWindowContainer() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual NewWindowInfo addWindow() = 0;
		virtual void removeWindow(WindowHandle handle) = 0;

		virtual IWindow* getWindow(WindowHandle handle) = 0;

		virtual bool isValidWindowHandle(WindowHandle handle) const = 0;
	};
}