#pragma once

#include "PluginSystem/ExternalPluginCore.h"

namespace VT_WIN32
{
	class Win32PluginPlatform final : public VT::PluginBase
	{
	public:
		Win32PluginPlatform(VT::PluginID id, VT_PLATFORM_HANDLE_MODULE nativeHandle)
			: VT::PluginBase(id, nativeHandle) {}

		virtual void onLoaded() override;
		virtual void onUnloaded() override;
	};
}