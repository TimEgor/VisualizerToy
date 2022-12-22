#pragma once

#include "VulkanGraphicsPlugin/VulkanCore.h"
#include "VulkanGraphicsPlugin/Textures/VulkanTexture2D.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedSwapChainObject.h"

#include <vector>

namespace VT_VK
{
	class VulkanGraphicDevice;

	struct VulkanSwapChainInfo final
	{
		VkSurfaceCapabilitiesKHR m_capabilities = VkSurfaceCapabilitiesKHR();
		std::vector<VkSurfaceFormatKHR> m_surfaceFormats;
		std::vector<VkPresentModeKHR> m_presentModes;
	};

	class VulkanSwapChain final : public VT::ManagedGraphicDevice::ManagedSwapChainBase
	{
		friend VulkanGraphicDevice;

	private:
		VkSwapchainKHR m_vkSwapChain = 0;
		VkSurfaceKHR m_vkSurface = 0;

		VulkanTexture2D* m_textures = nullptr;
		VulkanTexture2DView* m_targetViews = nullptr;
		uint32_t m_textureCount = 0;

	public:
		VulkanSwapChain(const VT::SwapChainDesc& desc, VkSwapchainKHR swapChain, VkSurfaceKHR surface)
			: ManagedSwapChainBase(desc), m_vkSwapChain(swapChain), m_vkSurface(surface) {}

		virtual uint32_t getTextureCount() const override;
		virtual const VT::ITexture2D* getTexture(uint32_t index) const override;
		virtual const VT::ITexture2DView* getTargetView(uint32_t index) const override;

		VkSwapchainKHR getVkSwapChain() const { return m_vkSwapChain; }
		VkSurfaceKHR getVkSurface() const { return m_vkSurface; }
	};
}