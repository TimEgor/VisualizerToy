#pragma once

#include "Textures/ITextureViews.h"
#include "VulkanGraphicsPlugin/VulkanCore.h"

namespace VT_VK
{
	VkImageAspectFlags convertImageAspect_VT_to_VK(const VT::TextureViewAspect aspect)
	{
		switch (aspect) {
		case VT::TextureViewAspect::COLOR:
			return VK_IMAGE_ASPECT_COLOR_BIT;
		case VT::TextureViewAspect::DEPTH:
			return VK_IMAGE_ASPECT_DEPTH_BIT;
		case VT::TextureViewAspect::STENCIL:
			return VK_IMAGE_ASPECT_STENCIL_BIT;
		default:
			return VK_IMAGE_ASPECT_NONE;
		}
	}
}