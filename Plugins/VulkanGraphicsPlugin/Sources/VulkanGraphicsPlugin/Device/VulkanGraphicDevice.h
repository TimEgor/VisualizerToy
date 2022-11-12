#pragma once

#include "VulkanGraphicsPlugin/VulkanCore.h"
#include "ManagedGraphicDevice/ManagedGraphicDevice.h"
#include "VulkanDestroyingResourceCollection.h"

#define VT_GRAPHIC_DEVICE_VULKAN_TYPE

namespace VT
{
	struct Texture2DDesc;
}

namespace VT_VK
{
	struct VulkanSwapChainInfo;

	class VulkanSwapChain;

	class VulkanGraphicDevice final : public VT::ManagedGraphicDevice::ManagedGraphicDevice
	{
	private:
		VulkanDestroyingResourceCollection m_destroyingResources;

		VkDevice m_vkDevice = nullptr;
		VkPhysicalDevice m_vkPhysDevice = nullptr;

		VkQueue m_vkGraphicQueue = nullptr;
		VkQueue m_vkTransferQueue = nullptr;
		VkQueue m_vkComputeQueue = nullptr;

		VulkanQueueFamilyIndex m_graphicQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		VulkanQueueFamilyIndex m_transferQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		VulkanQueueFamilyIndex m_computeQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		bool initVkDevice(VkInstance vkInstance, bool isSwapChainEnabled);

		bool chooseVkPhysDevice(VkInstance vkInstance, const VulkanNameContainer& extensions);
		bool checkVkPhysDevice(VkPhysicalDevice device, const VulkanNameContainer& extensions);

		void findQueueFamiliesIndices();

		void getSwapChainCapabilitiesInfo(VkSurfaceKHR surface, VulkanSwapChainInfo& info);
		void createSwapChainInternal(const VT::SwapChainDesc& swapCahinDesc, const VT::IWindow* window, VkSurfaceKHR& surface, VkSwapchainKHR& swapChain, VT::Texture2DDesc& imageDesc);
		void initSwapChainImages(VulkanSwapChain* swapChain, const VT::Texture2DDesc& imageDesc);

		void destroyResources();

	protected:
		virtual bool initDevice(bool isSwapChainEnabled) override;
		virtual void releaseDevice() override;

		virtual bool createSwapChain(VT::ManagedGraphicDevice::ManagedSwapChainBase* swapChain, const VT::SwapChainDesc& desc, const VT::IWindow* window) override;
		virtual void destroySwapChain(VT::ManagedGraphicDevice::ManagedSwapChainBase* swapChain) override;

		virtual void destroyTexture2D(VT::ManagedGraphicDevice::ManagedTexture2DBase* texture) override;

		virtual VT::ManagedGraphicDevice::ManagedGraphicDevice::SwapChainStorage* createSwapChainStorage() const override;
		virtual VT::ManagedGraphicDevice::ManagedGraphicDevice::Texture2DStorage* createTexture2DStorage() const override;

	public:
		VulkanGraphicDevice() = default;

		virtual void update() override;

		virtual void wait() override;

		VT_GRAPHIC_DEVICE_TYPE(VT_GRAPHIC_DEVICE_VULKAN_TYPE)
	};
}