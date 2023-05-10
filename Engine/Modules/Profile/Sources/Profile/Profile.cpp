#include "Profile.h"

#if !defined(VT_CONFIG_RETAIL)
	#ifdef VT_PROFILE_PIX
		#include "Windows.h"

		#define USE_PIX
		#include "WinPixEventRuntime/pix3.h"
	#endif

	#ifdef VT_PROFILE_SUPERLUMINAL
		#include "Superluminal/PerformanceAPI.h"
	#endif
#endif

VT::Profile::ProfileEvent::ProfileEvent(const char* title, const char* context)
{
	beginEvent(title, context);
}

VT::Profile::ProfileEvent::~ProfileEvent()
{
	endEvent();
}

void VT::Profile::beginEvent(const char* titleName, const char* contextName)
{
#if !defined(VT_CONFIG_RETAIL)
#ifdef VT_PROFILE_PIX
	PIXBeginEvent(PIX_COLOR_DEFAULT, "%s: %s", titleName, contextName ? contextName : "");
#endif
#ifdef VT_PROFILE_SUPERLUMINAL
	PerformanceAPI_BeginEvent(titleName, contextName, PERFORMANCEAPI_DEFAULT_COLOR);
#endif
#endif
}

void VT::Profile::endEvent()
{
#if !defined(VT_CONFIG_RETAIL)
#ifdef VT_PROFILE_SUPERLUMINAL
	PerformanceAPI_EndEvent();
#endif

#ifdef VT_PROFILE_PIX
	PIXEndEvent();
#endif
#endif
}
