#pragma once

#include "VulkanGraphicsPlugin/Device/VulkanDeviceObject.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedBuffer.h"

namespace VT_VK
{
	class VulkanGPUBuffer final : public VT::ManagedGraphicDevice::ManagedGPUBufferBase, public VulkanDeviceObject
	{
		friend class VulkanGraphicDevice;

	private:
		VkBuffer m_vkBuffer = 0;
		VkDeviceMemory m_vkMemory = 0;

	public:
		VulkanGPUBuffer(const VT::GPUBufferDesc& desc, VkDevice vkDevice,
			VkBuffer vkBuffer, VkDeviceMemory vkDeviceMemory)
			: ManagedGPUBufferBase(desc), VulkanDeviceObject(vkDevice),
			m_vkBuffer(vkBuffer), m_vkMemory(vkDeviceMemory) {}

		virtual void mapData(void** data) override;
		virtual void unmapData() override;

		VkBuffer getVkBuffer() const { return m_vkBuffer; }
		VkDeviceMemory getVkDeviceMemory() const { return m_vkMemory; }
	};
}
