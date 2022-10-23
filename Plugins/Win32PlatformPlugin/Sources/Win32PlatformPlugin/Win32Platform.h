#pragma once

#include "Core/Platform.h"
#include "Platform/IPlatform.h"

#define VT_PLARFORM_WIN32_TYPE

class Win32Platform final : public VT::IPlatform
{
private:
	HINSTANCE m_hInstance;

public:
	Win32Platform(HINSTANCE hInstance)
		: m_hInstance(hInstance) {}

	HINSTANCE getHInstance() const { return m_hInstance; }

	virtual VT::IWindowSystem* createWindowSystem() override;

	VT_PLATFORM_TYPE(VT_PLARFORM_WIN32_TYPE)
};