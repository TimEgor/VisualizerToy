#pragma once

#include "Core/CommonMacros.h"

namespace VT
{
	namespace Profile
	{
		class ProfileEvent final
		{
		public:
			ProfileEvent(const char* title, const char* context = nullptr);
			~ProfileEvent();
		};

		void beginEvent(const char* titleName, const char* contextName = nullptr);
		void endEvent();
	}
}

#if !defined(VT_CONFIG_RETAIL)
	#define VT_PROFILE_SCOPE_NAME(SCOPE) VT_CONCAT(profileEvent_, SCOPE)
	#define VT_PROFILE_GENERIC_SCOPE_NAME() VT_PROFILE_SCOPE_NAME(VT_COUNTER)

	#define VT_PROFILE_BEGIN_EVENT(TITLE) VT::Profile::beginEvent(TITLE);
	#define VT_PROFILE_BEGIN_EVENT_CONTEXT(TITLE, CONTEXT) VT::Profile::beginEvent(TITLE, CONTEXT);
	#define VT_PROFILE_END_EVENT() VT::Profile::endEvent();

	#define VT_PROFILE_BLOCK_EVENT(TITLE) VT::Profile::ProfileEvent VT_PROFILE_GENERIC_SCOPE_NAME()(TITLE);
	#define VT_PROFILE_BLOCK_EVENT_CONTEXT(TITLE, CONTEXT) VT::Profile::ProfileEvent VT_PROFILE_GENERIC_SCOPE_NAME()(TITLE, CONTEXT);
	#define VT_PROFILE_FUNCTION() VT::Profile::createBlockEvent(VT_FUNCTION_NAME);
#else
	#define VT_PROFILE_BEGIN_EVENT(NAME)
	#define VT_PROFILE_END_EVENT()

	#define VT_PROFILE_BLOCK_EVENT(NAME)
	#define VT_PROFILE_FUNCTION()
#endif
