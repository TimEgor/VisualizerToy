#pragma once

#include "IWindow.h"
#include "IWindowEventSystem.h"

namespace VT
{
	class IWindowSystem
	{
	public:
		IWindowSystem() = default;
		virtual ~IWindowSystem() {}

		virtual bool init(IWindowEventSystem* eventSystem) = 0;
		virtual void release() = 0;

		virtual IWindow* createWindow(const char* title, const WindowSize& size) = 0;
		virtual void destroyWindow(IWindow* window) = 0;

		virtual void updateWindowEvents() = 0;
	};
}