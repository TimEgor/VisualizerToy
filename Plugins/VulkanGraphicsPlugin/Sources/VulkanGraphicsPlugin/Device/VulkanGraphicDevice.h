#pragma once

#include "GraphicDevice/IGraphicDevice.h"
#include "VulkanGraphicsPlugin/VulkanCore.h"

#include <vector>

#define VT_GRAPHIC_DEVICE_VULKAN_TYPE

namespace VT_VK
{
	using VulkanQueueFamilyIndex = uint32_t;

	using VulkanPhysDevicesContainer = std::vector<VkPhysicalDevice>;
	using VulkanQueueFamilyPropertiesContainer = std::vector<VkQueueFamilyProperties>;
	using VulkanLayerNameContainer = std::vector<const char*>;

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

		bool initVkInstance();
		bool initVkDevice();

		bool chooseVkPhysDevice();
		bool checkVkPhysDevice(VkPhysicalDevice device);

		void findQueueFamiliesIndices();

	public:
		VulkanGraphicDevice() = default;
		virtual ~VulkanGraphicDevice() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual void wait() override;

		VT_GRAPHIC_DEVICE_TYPE(VT_GRAPHIC_DEVICE_VULKAN_TYPE)
	};
}