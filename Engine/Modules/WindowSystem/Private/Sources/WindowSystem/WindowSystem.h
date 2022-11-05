#pragma once

#include "WindowSystem/IWindowSystem.h"

namespace VT
{
	class WindowSystem : public IWindowSystem
	{
	private:
		IWindowContainer* m_windowContainer = nullptr;
		IWindowEventSystem* m_eventSystem = nullptr;

	public:
		WindowSystem() = default;

		virtual bool init(IWindowContainer* container, IWindowEventSystem* eventSystem) override;
		virtual void release() override;

		virtual IWindowContainer::NewWindowInfo createWindow(const char* title, const WindowSize& size) override;
		virtual void destroyWindow(WindowHandle handle) override;

		virtual bool isValidWindowHandle(WindowHandle handle) const override;

		virtual void updateWindowEvents() override;
	};
}