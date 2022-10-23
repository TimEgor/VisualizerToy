#pragma once

#include "IWindow.h"
#include "ObjectPool/ObjectPoolHandle.h"

namespace VT
{
	using WindowHandle = ObjectPoolHandle32;

	class IWindowSystem
	{
	public:
		IWindowSystem() = default;
		virtual ~IWindowSystem() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual WindowHandle createWindow(const char* title, const WindowSize& size) = 0;
		virtual void destroyWindow(WindowHandle handle) = 0;

		virtual bool isValidWindowHandle(WindowHandle handle) const = 0;
	};
}