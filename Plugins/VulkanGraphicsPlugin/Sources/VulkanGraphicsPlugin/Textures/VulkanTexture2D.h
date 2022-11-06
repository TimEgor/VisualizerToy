#pragma once

#include "VulkanGraphicsPlugin/VulkanCore.h"
#include "Textures/Texture2DBase.h"

namespace VT_VK
{
	class VulkanTexture2D final : public VT::Texture2DBase
	{
	private:
		VkImage m_vkImage = 0;

	public:
		VulkanTexture2D(VkImage image, const VT::Texture2DDesc& desc)
			: VT::Texture2DBase(desc), m_vkImage(image) {}
	};
}