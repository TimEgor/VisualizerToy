#pragma once

#include "Core/Platform.h"
#include "Platform/IPlatform.h"

#define VT_PLARFORM_WIN32_TYPE

namespace VT_WIN32
{
	class Win32Platform final : public VT::IPlatform
	{
	private:
		HINSTANCE m_hInstance;

		bool initWindowClass();

	public:
		Win32Platform(HINSTANCE hInstance)
			: m_hInstance(hInstance) {}

		virtual bool init() override;
		virtual void release() override {}

		HINSTANCE getHInstance() const { return m_hInstance; }

		//WindowSystem
		virtual VT::IWindowContainer* createWindowContainer() override;
		virtual VT::IWindowEventSystem* createWindowEventSystem() override;

		VT_PLATFORM_TYPE(VT_PLARFORM_WIN32_TYPE)
	};
}