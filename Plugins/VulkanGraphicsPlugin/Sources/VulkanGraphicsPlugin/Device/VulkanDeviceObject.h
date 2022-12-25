#pragma once

#include "VulkanGraphicsPlugin/VulkanCore.h"

namespace VT_VK
{
	class VulkanDeviceObject
	{
	private:
		VkDevice m_vkDevice = 0;

	protected:
		VkDevice getVkDevice() const { return m_vkDevice; }

	public:
		VulkanDeviceObject(VkDevice vkDevice)
			: m_vkDevice(vkDevice) {}
	};
}
