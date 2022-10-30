#pragma once

#include "IWindowContainer.h"
#include "IWindowEventSystem.h"

namespace VT
{
	class IWindowSystem
	{
	public:
		IWindowSystem() = default;
		virtual ~IWindowSystem() {}

		virtual bool init(IWindowContainer* container, IWindowEventSystem* eventSystem) = 0;
		virtual void release() = 0;

		virtual WindowHandle createWindow(const char* title, const WindowSize& size) = 0;
		virtual void destroyWindow(WindowHandle handle) = 0;

		virtual bool isValidWindowHandle(WindowHandle handle) const = 0;

		virtual void updateWindowEvents() = 0;
	};
}