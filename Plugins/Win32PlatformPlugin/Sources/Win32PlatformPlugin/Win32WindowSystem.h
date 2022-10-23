#pragma once

#include "WindowSystem/WindowSystemBase.h"
#include "Win32Window.h"

class Win32WindowSystem final : public VT::WindowSystemBase<Win32Window>
{
private:
	static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	Win32WindowSystem() = default;
	~Win32WindowSystem() { release(); }

	virtual bool init() override;
	virtual void release() override;

	virtual void updateWindowEvents() override;

	bool registerClass();
};