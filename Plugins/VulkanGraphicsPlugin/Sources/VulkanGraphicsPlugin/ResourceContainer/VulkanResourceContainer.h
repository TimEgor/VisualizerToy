#pragma once

#include "GraphicResourceContainerObjectPool/ObjectPoolResourceContainer.h"

#include "VulkanGraphicsPlugin/SwapChain/VulkanSwapChain.h"
#include "VulkanGraphicsPlugin/Textures/VulkanTexture2D.h"

#include <type_traits>

namespace VT_VK
{
	using VulkanSwapChainContainer = VT::SwapChainObjectPoolContainer<VulkanSwapChain>;
	using VulkanTexture2DContainer = VT::Texture2DObjectPoolContainer<VulkanTexture2D>;
}