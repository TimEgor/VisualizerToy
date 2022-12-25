#pragma once

#include "VulkanGraphicsPlugin/VulkanCore.h"
#include "VulkanGraphicsPlugin/Textures/VulkanTexture2D.h"
#include "VulkanGraphicsPlugin/Device/VulkanDeviceObject.h"
#include "VulkanGraphicsPlugin/Synchronization/VulkanSemaphore.h"
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

	class VulkanSwapChain final : public VT::ManagedGraphicDevice::ManagedSwapChainBase, public VulkanDeviceObject
	{
		friend VulkanGraphicDevice;

	private:
		VkSwapchainKHR m_vkSwapChain = 0;
		VkSurfaceKHR m_vkSurface = 0;

		VkQueue m_queue = 0;

		VulkanSemaphore m_textureAvailableSemaphore;
		VulkanTexture2D* m_textures = nullptr;
		VulkanTexture2DView* m_targetViews = nullptr;
		uint32_t m_textureCount = 0;

		uint32_t m_currentTextureIndex = 0;

	public:
		VulkanSwapChain(const VT::SwapChainDesc& desc, VkDevice vkDevice,
			VkSwapchainKHR swapChain, VkSurfaceKHR surface, VkQueue presentQueue, VkSemaphore vkTextureAvailSemaphore)
			: ManagedSwapChainBase(desc), VulkanDeviceObject(vkDevice),
			m_vkSwapChain(swapChain), m_vkSurface(surface), m_queue(presentQueue),
			m_textureAvailableSemaphore(vkTextureAvailSemaphore) {}

		virtual uint32_t getTextureCount() const override;
		virtual VT::ITexture2D* getTargetTexture(uint32_t index) override;
		virtual VT::ITexture2DView* getTargetTextureView(uint32_t index) override;

		virtual VT::ISemaphore* getTextureAvailableSemaphore() override;

		virtual void updateCurrentTextureIndex() override;
		virtual uint32_t getCurrentTextureIndex() const override { return m_currentTextureIndex; }

		virtual void present(VT::ISemaphore* readyPresentSemaphore) override;

		VkSwapchainKHR getVkSwapChain() const { return m_vkSwapChain; }
		VkSurfaceKHR getVkSurface() const { return m_vkSurface; }
	};
}
