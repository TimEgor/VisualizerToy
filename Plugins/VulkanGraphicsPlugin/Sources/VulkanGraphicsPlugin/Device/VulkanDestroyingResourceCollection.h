#pragma once

#include "VulkanGraphicsPlugin/VulkanCore.h"

#include <vector>

namespace VT_VK
{
	template <typename VkResourceType>
	using VulkanDestroyingResourceContainer = std::vector<VkResourceType>;

	struct VulkanDestroyingResourceCollection final
	{
		VulkanDestroyingResourceContainer<VkSwapchainKHR> m_swapChains;
		VulkanDestroyingResourceContainer<VkSurfaceKHR> m_surfaces;
	};
}