#include "VulkanSwapChain.h"

#include "VulkanGraphicsPlugin/Device/VulkanGraphicDevice.h"

uint32_t VT_VK::VulkanSwapChain::getTextureCount() const
{
	return m_textureCount;
}

VT::ITexture2D* VT_VK::VulkanSwapChain::getTargetTexture(uint32_t index)
{
	assert(m_textures);
	return &m_textures[index];
}

VT::ITexture2DView* VT_VK::VulkanSwapChain::getTargetTextureView(uint32_t index)
{
	assert(m_targetViews);
	return &m_targetViews[index];
}

VT::ISemaphore* VT_VK::VulkanSwapChain::getTextureAvailableSemaphore()
{
	return &m_textureAvailableSemaphore;
}

void VT_VK::VulkanSwapChain::updateCurrentTextureIndex()
{
	checkVkResultAssert(vkAcquireNextImageKHR(getVkDevice(), m_vkSwapChain, UINT64_MAX, m_textureAvailableSemaphore.getVkSemaphore(), VK_NULL_HANDLE, &m_currentTextureIndex));
}

void VT_VK::VulkanSwapChain::present(VT::ISemaphore* readyPresentSemaphore)
{
	VkSemaphore waitingSemaphore = readyPresentSemaphore ?
		reinterpret_cast<VulkanSemaphore*>(readyPresentSemaphore)->getVkSemaphore() :
		VK_NULL_HANDLE;

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = waitingSemaphore != VK_NULL_HANDLE ? 1 : 0;
	presentInfo.pWaitSemaphores = &waitingSemaphore;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &m_vkSwapChain;
	presentInfo.pImageIndices = &m_currentTextureIndex;

	vkQueuePresentKHR(m_queue, &presentInfo);
}
