#pragma once

#include "VulkanGraphicsPlugin/VulkanCore.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedCommandPoolObject.h"

namespace VT_VK
{
	class VulkanCommandPool final : public VT::ManagedGraphicDevice::ManagedCommandPoolBase
	{
	private:
		VkCommandPool m_vkCommandPool;

	public:
		VulkanCommandPool(VkCommandPool vkCommandPool)
			: m_vkCommandPool(vkCommandPool) {}

		virtual void reset() override {}
	};
}