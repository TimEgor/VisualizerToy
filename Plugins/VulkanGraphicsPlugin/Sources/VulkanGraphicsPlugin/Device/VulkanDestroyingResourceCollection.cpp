#include "VulkanDestroyingResourceCollection.h"

void VT_VK::VulkanDestroyingResourceCollection::destroyResources(VkInstance vkInstance, VkDevice vkDevice)
{
	m_fences.getLocker().lock();
	for (VkFence vkFence : m_fences.getContainer())
	{
		vkDestroyFence(vkDevice, vkFence, nullptr);
	}
	m_fences.getContainer().clear();

	m_fences.getLocker().unlock();

	//

	m_semaphores.getLocker().lock();
	for (VkSemaphore vkSemaphore : m_semaphores.getContainer())
	{
		vkDestroySemaphore(vkDevice, vkSemaphore, nullptr);
	}
	m_semaphores.getContainer().clear();

	m_semaphores.getLocker().unlock();


	//

	m_commandPools.getLocker().lock();
	for (VkCommandPool vkPool : m_commandPools.getContainer())
	{
		vkDestroyCommandPool(vkDevice, vkPool, nullptr);
	}
	m_commandPools.getContainer().clear();

	m_commandPools.getLocker().unlock();

	//

	m_images.getLocker().lock();
	for (VkImage vkImage : m_images.getContainer())
	{
		vkDestroyImage(vkDevice, vkImage, nullptr);
	}
	m_images.getContainer().clear();

	m_images.getLocker().unlock();

	//

	m_imageViews.getLocker().lock();
	for (VkImageView vkImageView : m_imageViews.getContainer())
	{
		vkDestroyImageView(vkDevice, vkImageView, nullptr);
	}
	m_imageViews.getContainer().clear();

	m_imageViews.getLocker().unlock();

	//

	m_pipelineLayouts.getLocker().lock();
	for (VkPipelineLayout vkPipelineLayout : m_pipelineLayouts.getContainer())
	{
		vkDestroyPipelineLayout(vkDevice, vkPipelineLayout, nullptr);
	}
	m_pipelineLayouts.getContainer().clear();

	m_pipelineLayouts.getLocker().unlock();

	//

	m_pipelines.getLocker().lock();
	for (VkPipeline vkPipeline : m_pipelines.getContainer())
	{
		vkDestroyPipeline(vkDevice, vkPipeline, nullptr);
	}
	m_pipelines.getContainer().clear();

	m_pipelines.getLocker().unlock();

	//

	m_shaderModules.getLocker().lock();
	for (VkShaderModule vkModule : m_shaderModules.getContainer())
	{
		vkDestroyShaderModule(vkDevice, vkModule, nullptr);
	}
	m_shaderModules.getContainer().clear();

	m_shaderModules.getLocker().unlock();

	//

	m_renderPasses.getLocker().lock();
	for (VkRenderPass vkPass : m_renderPasses.getContainer())
	{
		vkDestroyRenderPass(vkDevice, vkPass, nullptr);
	}
	m_renderPasses.getContainer().clear();

	m_renderPasses.getLocker().unlock();

	//

	m_swapChains.getLocker().lock();
	for (VkSwapchainKHR vkSwapChain : m_swapChains.getContainer())
	{
		vkDestroySwapchainKHR(vkDevice, vkSwapChain, nullptr);
	}
	m_swapChains.getContainer().clear();

	m_swapChains.getLocker().unlock();

	//

	m_surfaces.getLocker().lock();
	for (VkSurfaceKHR vkSurface : m_surfaces.getContainer())
	{
		vkDestroySurfaceKHR(vkInstance, vkSurface, nullptr);
	}
	m_surfaces.getContainer().clear();

	m_surfaces.getLocker().unlock();
}
