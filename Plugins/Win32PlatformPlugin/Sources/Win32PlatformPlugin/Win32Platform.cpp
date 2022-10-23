#include "Win32Platform.h"

#include "Win32WindowSystem.h"

VT::IWindowSystem* Win32Platform::createWindowSystem()
{
    return new Win32WindowSystem();
}
