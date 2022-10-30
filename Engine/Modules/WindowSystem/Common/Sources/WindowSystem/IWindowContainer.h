#pragma once

#include "IWindow.h"
#include "ObjectPool/ObjectPoolHandle.h"

namespace VT
{
	using WindowHandle = ObjectPoolHandle32;

	class IWindowContainer
	{
	public:
		struct NewWindowInfo final
		{
			WindowHandle m_windowHandle = WindowHandle::InvalidKey;
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