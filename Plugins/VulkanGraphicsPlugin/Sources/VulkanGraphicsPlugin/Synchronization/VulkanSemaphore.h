#pragma once

#include "VulkanGraphicsPlugin/VulkanCore.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedSyncObjects.h"

namespace VT_VK
{
	class VulkanSemaphore final : public VT::ManagedGraphicDevice::ManagedSemaphoreBase
	{
		friend class VulkanGraphicDevice;

	private:
		VkSemaphore m_vkSemaphore = 0;

	public:
		VulkanSemaphore(VkSemaphore vkSemaphore)
			: m_vkSemaphore(vkSemaphore) {}

		VkSemaphore getVkSemaphore() const { return m_vkSemaphore; }
	};
}