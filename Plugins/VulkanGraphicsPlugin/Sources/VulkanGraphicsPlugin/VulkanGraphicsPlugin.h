#pragma once

#include "PluginSystem/ExternalPluginCore.h"

class VulkanGraphicPlugin final : public VT::PluginBase
{
public:
	VulkanGraphicPlugin(VT::PluginID id, VT_PLATFORM_HANDLE_MODULE nativeHandle)
		: VT::PluginBase(id, nativeHandle) {}

	virtual void onLoaded() override;
	virtual void onUnloaded() override;
};