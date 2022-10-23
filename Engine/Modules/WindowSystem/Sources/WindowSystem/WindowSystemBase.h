#pragma once

#include "IWindowSystem.h"
#include "ObjectPool/ObjectPool.h"

namespace VT
{
	template <typename WindowType>
	class WindowSystemBase : public IWindowSystem
	{
		using WindowPool = ObjectPool<WindowType, WindowHandle>;

	private:
		WindowPool m_windowPool;

	protected:
		bool initWindowPool(size_t pageSize = 4, size_t maxFreePageCount = 0, size_t minFreeIndexCount = 32)
		{
			return m_windowPool.init(pageSize, maxFreePageCount, minFreeIndexCount);
		}
		void releaseWindowPool()
		{
			m_windowPool.release();
		}

	public:
		WindowSystemBase() = default;

		virtual WindowHandle createWindow(const char* title, const WindowSize& size) override
		{
			WindowHandle handle = m_windowPool.addElement();
			IWindow* window = m_windowPool.getElement(handle);
			window->init(title, size);

			return handle;
		}
		virtual void destroyWindow(WindowHandle handle) override
		{
			IWindow* window = m_windowPool.getElement(handle);
			if (window)
			{
				window->release();
				m_windowPool.removeElement(handle);
			}
		}

		virtual bool isValidWindowHandle(WindowHandle handle) const override { return m_windowPool.isValid(handle); }
	};
}