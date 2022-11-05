#pragma once

#include "VulkanGraphicsPlugin/VulkanCore.h"
#include "SwapChain/SwapChainBase.h"

#include <vector>

namespace VT_VK
{
	struct VulkanSwapChainInfo final
	{
		VkSurfaceCapabilitiesKHR m_capabilities = VkSurfaceCapabilitiesKHR();
		std::vector<VkSurfaceFormatKHR> m_surfaceFormats;
		std::vector<VkPresentModeKHR> m_presentModes;
	};

	class VulkanSwapChain final : public VT::SwapChainBase
	{
	private:
		VkSwapchainKHR m_vkSwapChain = 0;
		VkSurfaceKHR m_vkSurface = 0;

	public:
		VulkanSwapChain(VkSwapchainKHR swapChain, VkSurfaceKHR surface, const VT::SwapChainDesc& desc)
			: VT::SwapChainBase(desc), m_vkSwapChain(swapChain), m_vkSurface(surface) {}


	};
}