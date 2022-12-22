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
			: ManagedTexture2DBase(desc), m_vkImage(image) {}

		VkImage getVkImage() const { return m_vkImage; }
	};

	class VulkanTexture2DView final : public VT::ManagedGraphicDevice::ManagedTexture2DViewBase
	{
		friend VulkanGraphicDevice;

	private:
		VkImageView m_vkImageView = 0;

	public:
		VulkanTexture2DView(const VT::TextureViewDesc& desc, VkImageView imageView)
			: ManagedTexture2DViewBase(desc), m_vkImageView(imageView) {}

		VkImageView getVkImageView() const { return m_vkImageView; }
	};
}