#pragma once

#include "GraphicCommandResources/ICommandQueue.h"
#include "VulkanGraphicsPlugin/VulkanCore.h"

namespace VT_VK
{
	class VulkanCommandQueue final : public VT::ICommandQueue
	{
	private:
		VkQueue m_vkQueue = nullptr;

	public:
		VulkanCommandQueue(VkQueue vkQueue)
			: m_vkQueue(vkQueue) {}

		VkQueue getVkQueue() const { return m_vkQueue; }
	};
}