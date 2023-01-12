#pragma once

#include "GPUBuffer/IGPUBuffer.h"
#include "VulkanGraphicsPlugin/VulkanCore.h"

namespace VT_VK
{
	VkBufferUsageFlags converteBufferUsageVTtoVK(VT::GPUBufferUsageType usage)
	{
		VkBufferUsageFlags vkUsage = 0;

		uint32_t usageVal = static_cast<uint32_t>(usage);

		if (usageVal & static_cast<uint32_t>(VT::GPUBufferUsageType::VERTEX_BUFFER))
		{
			vkUsage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		}

		if (usageVal & static_cast<uint32_t>(VT::GPUBufferUsageType::INDEX_BUFFER))
		{
			vkUsage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		}

		return vkUsage;
	}
}