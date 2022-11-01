#pragma once

#include "IWindowContainer.h"
#include "ObjectPool/ObjectPool.h"

namespace VT
{
	template <typename WindowType>
	class WindowContainerBase : public IWindowContainer
	{
		using WindowPoolHandle = ObjectPoolHandle32;
		using WindowPool = ObjectPool<WindowType, WindowPoolHandle>;

		static constexpr size_t DefaultWindowPoolPageSize = 4;
		static constexpr size_t DefaultWindowPoolMaxFreePageCount = 0;
		static constexpr size_t DefaultWindowPoolMinFreeIndexCount = 3;

	private:
		WindowPool m_windowPool;

	public:
		WindowContainerBase() = default;

		virtual bool init() override { return m_windowPool.init(DefaultWindowPoolPageSize, DefaultWindowPoolMaxFreePageCount, DefaultWindowPoolMinFreeIndexCount); }
		virtual void release() override { m_windowPool.release(); }

		virtual NewWindowInfo addWindow() override
		{
			const WindowPool::NewElementInfo info = m_windowPool.addElement();
			return NewWindowInfo{ info.m_elementHandle.getKey(), info.m_elementPtr };
		}
		virtual void removeWindow(WindowHandle handle) override { m_windowPool.removeElement(handle); }

		virtual IWindow* getWindow(WindowHandle handle) override { return m_windowPool.getElement(handle); }

		virtual bool isValidWindowHandle(WindowHandle handle) const override { return m_windowPool.isValid(handle); }
	};
}