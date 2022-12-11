#pragma once

#include "VulkanGraphicsPlugin/VulkanCore.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedTexture2DObject.h"

namespace VT_VK
{
	class VulkanGraphicDevice;

	class VulkanTexture2D final : public VT::ManagedGraphicDevice::ManagedTexture2DBase
	{
		friend VulkanGraphicDevice;

	private:
		VkImage m_vkImage = 0;

	public:
		VulkanTexture2D(const VT::Texture2DDesc& desc, VkImage image)
			: VT::ManagedGraphicDevice::ManagedTexture2DBase(desc), m_vkImage(image) {}
	};
}