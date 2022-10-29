#include "Win32Platform.h"

#include "Win32WindowSystem.h"

VT::IWindowSystem* VT_WIN32::Win32Platform::createWindowSystem()
{
	return new Win32WindowSystem();
}
