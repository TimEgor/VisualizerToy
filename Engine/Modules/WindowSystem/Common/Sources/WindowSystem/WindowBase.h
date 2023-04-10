#pragma once

#include "IWindow.h"

namespace VT
{
	class WindowBase : public IWindow
	{
	protected:
		WindowSize m_windowSize = DefaultWindowSize;
		WindowSize m_clientAreaSize = DefaultWindowSize;
		WindowArea m_cutoutsArea = DefaultWindowArea;

		InstancedEventID m_sizeEventID = InvalidInstancedEventID;

		bool m_isShown = false;

		bool initEvents();
		void releaseEvents();

	public:
		WindowBase() = default;

		virtual bool isShown() const override { return m_isShown; }

		virtual const WindowSize& getWindowSize() const override { return m_windowSize; }
		virtual const WindowSize& getClientAreaSize() const override { return m_clientAreaSize; }
		virtual const WindowArea& getCutoutsArea() const override { return m_cutoutsArea; }

		virtual InstancedEventID getWindowSizeEventID() const override { return m_sizeEventID; }
	};
}