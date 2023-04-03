#pragma once

#include "Core/Platform.h"
#include "Platform/IPlatform.h"

#include <vector>

#define VT_PLARFORM_WIN32_TYPE

namespace VT_WIN32
{
	class Win32Platform final : public VT::IPlatform
	{
		using MonitorInfoCollection = std::vector<VT::MonitorInfo>;

	private:
		MonitorInfoCollection m_monitorInfos;

		HINSTANCE m_hInstance;

		bool initWindowClass();
		void collectMonitorInfo();

	public:
		Win32Platform(HINSTANCE hInstance)
			: m_hInstance(hInstance) {}

		virtual bool init() override;
		virtual void release() override {}

		HINSTANCE getHInstance() const { return m_hInstance; }
		virtual void* getNativeHandle() const override { return m_hInstance; }

		//WindowSystem
		virtual VT::IWindow* createWindow() override;
		virtual VT::IWindowEventSystem* createWindowEventSystem() override;

		virtual VT::IInputSystem* createInputSystem() override;

		virtual uint32_t getMonitorCount() const override { return m_monitorInfos.size(); }
		virtual const VT::MonitorInfo& getMonitorInfo(uint32_t index) const override { return m_monitorInfos[index]; }

		VT_PLATFORM_TYPE(VT_PLARFORM_WIN32_TYPE)
	};
}