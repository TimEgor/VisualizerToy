#pragma once

#include "PluginSystem/ExternalPluginCore.h"

class TestPlugin final : public VT::PluginBase
{
public:
	TestPlugin(VT::PluginID id, VT_PLATFORM_HANDLE_MODULE nativeHandle)
		: VT::PluginBase(id, nativeHandle) {}

	virtual void onLoaded() override;
	virtual void onUnloaded() override;
};