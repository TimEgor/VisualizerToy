#include "Profile.h"

#if !defined(VT_CONFIG_RETAIL)
	#ifdef VT_PROFILE_PIX
		#include "Windows.h"

		#define USE_PIX
		#include "WinPixEventRuntime/pix3.h"
	#endif
#endif

VT::Profile::ProfileEvent::ProfileEvent(const char* name)
{
	beginEvent(name);
}

VT::Profile::ProfileEvent::~ProfileEvent()
{
	endEvent();
}

VT::Profile::ProfileEvent VT::Profile::createBlockEvent(const char* eventName)
{
	return ProfileEvent(eventName);
}

void VT::Profile::beginEvent(const char* eventName)
{
#if !defined(VT_CONFIG_RETAIL)
	PIXBeginEvent(PIX_COLOR_DEFAULT, eventName);
#endif
}

void VT::Profile::endEvent()
{
#if !defined(VT_CONFIG_RETAIL)
	PIXEndEvent();
#endif
}
