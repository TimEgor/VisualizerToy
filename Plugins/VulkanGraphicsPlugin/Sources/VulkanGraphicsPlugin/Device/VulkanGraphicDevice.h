#pragma once

#include "GraphicDevice/IGraphicDevice.h"

#define VT_GRAPHIC_DEVICE_VULKAN_TYPE

class VulkanGraphicDevice final : public VT::IGraphicDevice
{
public:
	VulkanGraphicDevice() = default;
	virtual ~VulkanGraphicDevice() { release(); }

	virtual bool init() override;
	virtual void release() override;

	VT_GRAPHIC_DEVICE_TYPE(VT_GRAPHIC_DEVICE_VULKAN_TYPE)
};