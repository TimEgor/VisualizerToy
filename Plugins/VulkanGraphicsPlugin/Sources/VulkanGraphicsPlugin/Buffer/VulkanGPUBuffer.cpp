#include "VulkanGPUBuffer.h"

void VT_VK::VulkanGPUBuffer::mapData(void** data)
{
	vkMapMemory(getVkDevice(), m_vkMemory, 0, getDesc().m_byteSize, 0, data);
}

void VT_VK::VulkanGPUBuffer::unmapData()
{
	vkUnmapMemory(getVkDevice(), m_vkMemory);
}
