#pragma once

#include "VulkanGraphicsPlugin/VulkanCore.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedCommandObjects.h"

namespace VT_VK
{
	class VulkanCommandPool final : public VT::ManagedGraphicDevice::ManagedCommandPoolBase
	{
		friend class VulkanGraphicDevice;

	private:
		VkCommandPool m_vkCommandPool;

	public:
		VulkanCommandPool(VkCommandPool vkCommandPool)
			: m_vkCommandPool(vkCommandPool) {}

		VkCommandPool getVkCommandPool() const { return m_vkCommandPool; }
	};

	class VulkanCommandList final : public VT::ManagedGraphicDevice::ManagedCommandListBase
	{
		friend class VulkanGraphicDevice;

	private:
		VkCommandBuffer m_vkCommandBuffer;

	public:
		VulkanCommandList(VkCommandBuffer vkCommandBuffer)
			: m_vkCommandBuffer(vkCommandBuffer) {}

		VkCommandBuffer getVkCommandBuffer() const { return m_vkCommandBuffer; }
	};
}