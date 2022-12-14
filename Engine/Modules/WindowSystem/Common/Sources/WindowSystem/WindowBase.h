#pragma once

#include "IWindow.h"

namespace VT
{
	class WindowBase : public IWindow
	{
	protected:
		WindowSize m_windowSize = {0, 0};
		WindowSize m_clientAreaSize = {0, 0};
		WindowArea m_cutoutsArea = {0, 0, 0, 0};

		bool m_isShown = false;

	public:
		WindowBase() = default;

		virtual bool isShown() const override { return m_isShown; }

		virtual const WindowSize& getWindowSize() const override { return m_windowSize; }
		virtual const WindowSize& getClientAreaSize() const override { return m_clientAreaSize; }
		virtual const WindowArea& getCutoutsArea() const override { return m_cutoutsArea; }
	};
}