#pragma once

#include "PluginSystem/ExternalPluginCore.h"

class Win32PluginPlatform final : public VT::PluginBase
{
public:
	Win32PluginPlatform(VT::PluginID id, VT_PLATFORM_HANDLE_MODULE nativeHandle)
		: VT::PluginBase(id, nativeHandle) {}

	virtual void onLoaded() override;
	virtual void onUnloaded() override;
};