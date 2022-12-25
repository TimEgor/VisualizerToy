#pragma once

#include "VulkanGraphicsPlugin/VulkanCore.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedSyncObjects.h"

namespace VT_VK
{
	class VulkanFence final : public VT::ManagedGraphicDevice::ManagedFenceBase
	{
		friend class VulkanGraphicDevice;

	private:
		VkFence m_vkFence = 0;

	public:
		VulkanFence(VkFence vkFence)
			: m_vkFence(vkFence) {}

		VkFence getVkFence() const { return m_vkFence; }
	};
}