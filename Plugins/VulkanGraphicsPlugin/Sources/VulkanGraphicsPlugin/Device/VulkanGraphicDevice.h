#pragma once

#include "GraphicDevice/IGraphicDevice.h"
#include "VulkanGraphicsPlugin/VulkanCore.h"

#define VT_GRAPHIC_DEVICE_VULKAN_TYPE

namespace VT_VK
{
	struct VulkanSwapChainInfo;

	class VulkanGraphicDevice final : public VT::IGraphicDevice
	{
	private:
		VkInstance m_vkInstance = nullptr;

		VkDevice m_vkDevice = nullptr;
		VkPhysicalDevice m_vkPhysDevice = nullptr;

		VkQueue m_vkGraphicQueue = nullptr;
		VkQueue m_vkTransferQueue = nullptr;
		VkQueue m_vkComputeQueue = nullptr;

		VulkanQueueFamilyIndex m_graphicQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		VulkanQueueFamilyIndex m_transferQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		VulkanQueueFamilyIndex m_computeQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		bool m_swapChainEnabled = false;

		bool initVkInstance(bool swapChainEnabled);
		bool initVkDevice(bool swapChainEnabled);

		bool chooseVkPhysDevice(const VulkanNameContainer& extensions);
		bool checkVkPhysDevice(VkPhysicalDevice device, const VulkanNameContainer& extensions);

		void findQueueFamiliesIndices();

		void getSwapChainCapabilitiesInfo(VkSurfaceKHR surface, VulkanSwapChainInfo& info);
		void createSwapChainInternal(const VT::SwapChainDesc& desc, VT::IWindow* window, VkSurfaceKHR& surface, VkSwapchainKHR& swapChain);

	public:
		VulkanGraphicDevice() = default;
		virtual ~VulkanGraphicDevice() { release(); }

		virtual bool init(bool swapChainEnabled = true) override;
		virtual void release() override;

		virtual void wait() override;

		virtual VT::ISwapChain* createSwapChain(const VT::SwapChainDesc& desc, VT::IWindow* window) override;
		virtual bool createSwapChain(const VT::SwapChainDesc& desc, VT::IWindow* window, void* swapChainPtr) override;

		VT_GRAPHIC_DEVICE_TYPE(VT_GRAPHIC_DEVICE_VULKAN_TYPE)
	};
}