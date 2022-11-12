#pragma once

#include "WindowSystem/IWindowSystem.h"

namespace VT
{
	class WindowSystem : public IWindowSystem
	{
	private:
		IWindowEventSystem* m_eventSystem = nullptr;

	public:
		WindowSystem() = default;

		virtual bool init(IWindowEventSystem* eventSystem) override;
		virtual void release() override;

		virtual IWindow* createWindow(const char* title, const WindowSize& size) override;
		virtual void destroyWindow(IWindow* window) override;

		virtual void updateWindowEvents() override;
	};
}