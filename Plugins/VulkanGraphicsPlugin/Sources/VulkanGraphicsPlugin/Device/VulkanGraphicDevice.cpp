#include "VulkanGraphicDevice.h"

#include "Core/UtilitiesMacros.h"
#include "Core/String/Format.h"

#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"
#include "Engine/EngineInstance.h"

#include "Platform/IPlatform.h"
#include "WindowSystem/IWindow.h"

#include "VulkanGraphicsPlugin/SwapChain/VulkanSwapChain.h"
#include "VulkanGraphicsPlugin/Commands/VulkanCommandPool.h"
#include "VulkanGraphicsPlugin/Shaders/VulkanShaders.h"
#include "VulkanGraphicsPlugin/PipelineState/VulkanPipelineState.h"
#include "VulkanGraphicsPlugin/PipelineState/VulkanRenderPass.h"

#include "VulkanGraphicsPlugin/Utilities/FormatConverter.h"
#include "VulkanGraphicsPlugin/Utilities/PresentModeConverter.h"

#include "VulkanGraphicsPlugin/Utilities/VulkanEnvironmentGraphicPlatform.h"

#include <algorithm>

namespace VT_VK
{
	bool enumeratePhysDevices(VkInstance instance, VulkanPhysDevicesContainer& devices)
	{
		uint32_t deviceCount = 0;

		VkResult vkResult;
		vkResult = vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		bool enumResult = checkVkResultReturnAssert(vkResult);

		devices.clear();

		if (enumResult && deviceCount > 0)
		{
			devices.resize(deviceCount);
			vkResult = vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
			enumResult = checkVkResultReturnAssert(vkResult);
		}

		return enumResult;
	}

	void enumerateQueueFamilyProperties(VkPhysicalDevice device, VulkanQueueFamilyPropertiesContainer& properties)
	{
		uint32_t queueCount = 0;

		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, nullptr);

		properties.clear();

		if (queueCount > 0)
		{
			properties.resize(queueCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, properties.data());
		}
	}

	bool checkAvaliableDeviceExtensionsSupport(VkPhysicalDevice device, const VulkanNameContainer& extensions)
	{
		uint32_t extensionCount = 0;

		VkResult vkResult;
		vkResult = vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
		bool result = checkVkResultReturnAssert(vkResult);

		if (result && extensionCount > 0)
		{
			std::vector<VkExtensionProperties> availableExtensionProperties;
			availableExtensionProperties.resize(extensionCount);

			vkResult = vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensionProperties.data());
			result = checkVkResultReturnAssert(vkResult);

			if (result)
			{
				for (const char* extensionName : extensions)
				{
					bool isExtensionFound = false;

					for (const auto& availableExtensionProperty : availableExtensionProperties)
					{
						if (std::strncmp(extensionName, availableExtensionProperty.extensionName, VK_MAX_EXTENSION_NAME_SIZE) == 0)
						{
							isExtensionFound = true;
							break;
						}
					}

					if (!isExtensionFound)
					{
						result = false;
						break;
					}
				}
			}
		}

		return result;
	}
}

bool VT_VK::VulkanGraphicDevice::initVkDevice(VkInstance vkInstance, bool isSwapChainEnabled)
{
	VulkanNameContainer extensionNames;

	if (isSwapChainEnabled)
	{
		extensionNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	}

	VT_CHECK_RETURN_FALSE(chooseVkPhysDevice(vkInstance, extensionNames));

	findQueueFamiliesIndices();

	VT_CHECK_RETURN_FALSE(m_graphicQueueFamilyIndex != VK_QUEUE_FAMILY_IGNORED
		&& m_transferQueueFamilyIndex != VK_QUEUE_FAMILY_IGNORED
		&& m_computeQueueFamilyIndex != VK_QUEUE_FAMILY_IGNORED);

	std::vector<VulkanQueueFamilyIndex> requiringFamiliesQueue = { m_graphicQueueFamilyIndex, m_transferQueueFamilyIndex, m_computeQueueFamilyIndex };
	std::sort(requiringFamiliesQueue.begin(), requiringFamiliesQueue.end());
	requiringFamiliesQueue.erase(std::unique(requiringFamiliesQueue.begin(), requiringFamiliesQueue.end()), requiringFamiliesQueue.end());
	const uint32_t familyCount = static_cast<uint32_t>(requiringFamiliesQueue.size());

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfo(familyCount);

	const float queuePriority = 1.0f;
	for (uint32_t queueIndex = 0; queueIndex < familyCount; ++queueIndex)
	{
		VkDeviceQueueCreateInfo& queueInfo = queueCreateInfo[queueIndex];

		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.pQueuePriorities = &queuePriority;
		queueInfo.queueCount = 1;
		queueInfo.queueFamilyIndex = requiringFamiliesQueue[queueIndex];
	}

	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	deviceCreateInfo.queueCreateInfoCount = familyCount;
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfo.data();

	if (!extensionNames.empty())
	{
		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensionNames.size());
		deviceCreateInfo.ppEnabledExtensionNames = extensionNames.data();
	}

	VkResult creationResult = vkCreateDevice(m_vkPhysDevice, &deviceCreateInfo, nullptr, &m_vkDevice);
	VT_CHECK_RETURN_FALSE(checkVkResult(creationResult));

	vkGetDeviceQueue(m_vkDevice, m_graphicQueueFamilyIndex, 0, &m_vkGraphicQueue);
	vkGetDeviceQueue(m_vkDevice, m_transferQueueFamilyIndex, 0, &m_vkTransferQueue);
	vkGetDeviceQueue(m_vkDevice, m_computeQueueFamilyIndex, 0, &m_vkComputeQueue);

	VT_CHECK_RETURN_FALSE(m_vkGraphicQueue && m_vkTransferQueue && m_vkComputeQueue);

	return true;
}

bool VT_VK::VulkanGraphicDevice::chooseVkPhysDevice(VkInstance vkInstance, const VulkanNameContainer& extensions)
{
	assert(!m_vkPhysDevice);

	VulkanPhysDevicesContainer physDevices;
	VT_CHECK_RETURN_FALSE(enumeratePhysDevices(vkInstance, physDevices) && physDevices.size() > 0);

	for (auto device : physDevices)
	{
		if (checkVkPhysDevice(device, extensions))
		{
			m_vkPhysDevice = device;
			break;
		}
	}

	return m_vkPhysDevice;
}

bool VT_VK::VulkanGraphicDevice::checkVkPhysDevice(VkPhysicalDevice device, const VulkanNameContainer& extensions)
{
	//VkPhysicalDeviceProperties properties;
	//vkGetPhysicalDeviceProperties(device, &properties);

	//VkPhysicalDeviceFeatures features;
	//vkGetPhysicalDeviceFeatures(device, &features);

	bool result = true;

	if (!extensions.empty())
	{
		result &= checkAvaliableDeviceExtensionsSupport(device, extensions);
	}

	return result;
}

void VT_VK::VulkanGraphicDevice::findQueueFamiliesIndices()
{
	assert(m_vkPhysDevice);

	VulkanQueueFamilyPropertiesContainer queueFamiliesProperties;
	enumerateQueueFamilyProperties(m_vkPhysDevice, queueFamiliesProperties);
	uint32_t familyCount = static_cast<uint32_t>(queueFamiliesProperties.size());

	//Find first available queue families
	for (uint32_t queueFamilyIndex = 0; queueFamilyIndex < familyCount; ++queueFamilyIndex)
	{
		const VkQueueFamilyProperties queueFamilyProperties = queueFamiliesProperties[queueFamilyIndex];

		if (queueFamilyProperties.queueCount == 0)
		{
			continue;
		}

		if (m_graphicQueueFamilyIndex == VK_QUEUE_FAMILY_IGNORED && queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			m_graphicQueueFamilyIndex = queueFamilyIndex;
		}

		if (m_transferQueueFamilyIndex == VK_QUEUE_FAMILY_IGNORED && queueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT)
		{
			m_transferQueueFamilyIndex = queueFamilyIndex;
		}

		if (m_computeQueueFamilyIndex == VK_QUEUE_FAMILY_IGNORED && queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT)
		{
			m_computeQueueFamilyIndex = queueFamilyIndex;
		}
	}

	//Find dedicated queue families
	for (uint32_t queueFamilyIndex = 0; queueFamilyIndex < familyCount; ++queueFamilyIndex)
	{
		const VkQueueFamilyProperties queueFamilyProperties = queueFamiliesProperties[queueFamilyIndex];

		if (queueFamilyProperties.queueCount == 0)
		{
			continue;
		}

		if (m_transferQueueFamilyIndex == VK_QUEUE_FAMILY_IGNORED
			&& queueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT
			&& !(queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			&& !(queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT))
		{
			m_transferQueueFamilyIndex = queueFamilyIndex;
		}

		if (m_computeQueueFamilyIndex == VK_QUEUE_FAMILY_IGNORED
			&& queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT
			&& !(queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT))
		{
			m_computeQueueFamilyIndex = queueFamilyIndex;
		}
	}
}

void VT_VK::VulkanGraphicDevice::destroyResources()
{
	wait();

	VkInstance vkInstance = getVulkanEnvironmentGraphicPlatform()->getInstance();

	m_destroyingResources.m_images.getLocker().lock();
	for (VkImage vkImage : m_destroyingResources.m_images.getContainer())
	{
		vkDestroyImage(m_vkDevice, vkImage, nullptr);
	}
	m_destroyingResources.m_images.getLocker().unlock();

	m_destroyingResources.m_swapChains.getLocker().lock();
	for (VkSwapchainKHR vkSwapChain : m_destroyingResources.m_swapChains.getContainer())
	{
		vkDestroySwapchainKHR(m_vkDevice, vkSwapChain, nullptr);
	}
	m_destroyingResources.m_swapChains.getLocker().unlock();

	m_destroyingResources.m_surfaces.getLocker().lock();
	for (VkSurfaceKHR vkSurface : m_destroyingResources.m_surfaces.getContainer())
	{
		vkDestroySurfaceKHR(vkInstance, vkSurface, nullptr);
	}
	m_destroyingResources.m_surfaces.getLocker().unlock();

	m_destroyingResources.m_shaderModules.getLocker().lock();
	for (VkShaderModule vkModule : m_destroyingResources.m_shaderModules.getContainer())
	{
		vkDestroyShaderModule(m_vkDevice, vkModule, nullptr);
	}
	m_destroyingResources.m_shaderModules.getLocker().unlock();

	m_destroyingResources.m_pipelineLayouts.getLocker().lock();
	for (VkPipelineLayout vkPipelineLayout : m_destroyingResources.m_pipelineLayouts.getContainer())
	{
		vkDestroyPipelineLayout(m_vkDevice, vkPipelineLayout, nullptr);
	}
	m_destroyingResources.m_pipelineLayouts.getLocker().unlock();

	m_destroyingResources.m_pipelines.getLocker().lock();
	for (VkPipeline vkPipeline : m_destroyingResources.m_pipelines.getContainer())
	{
		vkDestroyPipeline(m_vkDevice, vkPipeline, nullptr);
	}
	m_destroyingResources.m_pipelines.getLocker().unlock();

	m_destroyingResources.m_renderPasses.getLocker().lock();
	for (VkRenderPass vkPass : m_destroyingResources.m_renderPasses.getContainer())
	{
		vkDestroyRenderPass(m_vkDevice, vkPass, nullptr);
	}
	m_destroyingResources.m_renderPasses.getLocker().unlock();
}

void VT_VK::VulkanGraphicDevice::getSwapChainCapabilitiesInfo(VkSurfaceKHR surface, VulkanSwapChainInfo& info)
{
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_vkPhysDevice, surface, &info.m_capabilities);

	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(m_vkPhysDevice, surface, &formatCount, nullptr);
	if (formatCount > 0)
	{
		info.m_surfaceFormats.clear();
		info.m_surfaceFormats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_vkPhysDevice, surface, &formatCount, info.m_surfaceFormats.data());
	}

	uint32_t presentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(m_vkPhysDevice, surface, &presentModeCount, nullptr);
	if (presentModeCount > 0)
	{
		info.m_presentModes.clear();
		info.m_presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_vkPhysDevice, surface, &presentModeCount, info.m_presentModes.data());
	}
}

void VT_VK::VulkanGraphicDevice::createSwapChainInternal(const VT::SwapChainDesc& swapChainDesc, const VT::IWindow* window, VkSurfaceKHR& surface, VkSwapchainKHR& swapChain, VT::Texture2DDesc& imageDesc)
{
	VT::IPlatform* platform = VT::EngineInstance::getInstance()->getEnvironment()->m_platform;
	VkInstance vkInstance = getVulkanEnvironmentGraphicPlatform()->getInstance();

	VT_CHECK_RETURN_ASSERT(window && platform);

	surface = 0;
	swapChain = 0;

#ifdef WIN32
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo{};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.hwnd = reinterpret_cast<HWND>(window->getNativeHandle());
	surfaceCreateInfo.hinstance = reinterpret_cast<HINSTANCE>(platform->getNativeHandle());

	vkCreateWin32SurfaceKHR(vkInstance, &surfaceCreateInfo, nullptr, &surface);
#endif

	VT_CHECK_RETURN_ASSERT(surface);

	VulkanSwapChainInfo capabilitiesInfo;
	getSwapChainCapabilitiesInfo(surface, capabilitiesInfo);

	bool checkProperty = false;
	VkFormat requiringVkFormat = convertFormat_VT_to_VK(swapChainDesc.m_format);
	VkColorSpaceKHR requiringVkColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	for (const VkSurfaceFormatKHR& availableFormat : capabilitiesInfo.m_surfaceFormats)
	{
		if (availableFormat.format == requiringVkFormat && availableFormat.colorSpace == requiringVkColorSpace)
		{
			checkProperty = true;
			break;
		}
	}

	VT_CHECK_RETURN_ASSERT_MSG(checkProperty, VT::stringFormat("Window surface doesn't support a requiring swap chain format (%d).", desc.m_format).c_str());

	checkProperty = false;
	VkPresentModeKHR requiringVkPresentMode = convertPresentMode_VT_to_VK(swapChainDesc.m_presentMode);
	for (VkPresentModeKHR availableMode : capabilitiesInfo.m_presentModes)
	{
		if (availableMode == requiringVkPresentMode)
		{
			checkProperty = true;
			break;
		}
	}

	VT_CHECK_RETURN_ASSERT_MSG(checkProperty, VT::stringFormat("Window surface doesn't support a requiring swap chain present mode (%d).", desc.m_presentMode).c_str());

	VT_CHECK_RETURN_ASSERT_MSG(capabilitiesInfo.m_capabilities.minImageCount <= swapChainDesc.m_imageCount
		&& swapChainDesc.m_imageCount <= capabilitiesInfo.m_capabilities.minImageCount,
		VT::stringFormat("Window surface doesn't support a requiring swap chain image count (%d).", desc.m_imageCount).c_str());

	VkSwapchainCreateInfoKHR swapChainCreateInfo{};
	swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapChainCreateInfo.surface = surface;
	swapChainCreateInfo.minImageCount = swapChainDesc.m_imageCount;
	swapChainCreateInfo.imageFormat = requiringVkFormat;
	swapChainCreateInfo.imageColorSpace = requiringVkColorSpace;
	swapChainCreateInfo.imageExtent = capabilitiesInfo.m_capabilities.currentExtent;
	swapChainCreateInfo.imageArrayLayers = 1;
	swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapChainCreateInfo.preTransform = capabilitiesInfo.m_capabilities.currentTransform;
	swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapChainCreateInfo.presentMode = requiringVkPresentMode;
	swapChainCreateInfo.clipped = true;

	checkVkResultAssert(vkCreateSwapchainKHR(m_vkDevice, &swapChainCreateInfo, nullptr, &swapChain));

	imageDesc.m_width = swapChainCreateInfo.imageExtent.width;
	imageDesc.m_height = swapChainCreateInfo.imageExtent.height;
	imageDesc.m_format = swapChainDesc.m_format;
}

void VT_VK::VulkanGraphicDevice::initSwapChainImages(VulkanSwapChain* swapChain, const VT::Texture2DDesc& imageDesc)
{
	assert(swapChain);
	assert(swapChain->m_vkSwapChain);
	assert(!swapChain->m_textures);

	uint32_t imageCount = 0;
	vkGetSwapchainImagesKHR(m_vkDevice, swapChain->m_vkSwapChain, &imageCount, nullptr);

	if (imageCount == 0)
	{
		return;
	}

	std::vector<VkImage> vkImages(imageCount);
	swapChain->m_textures = reinterpret_cast<VulkanTexture2D*>(new uint8_t[sizeof(VulkanTexture2D) * imageCount]);

	vkGetSwapchainImagesKHR(m_vkDevice, swapChain->m_vkSwapChain, &imageCount, vkImages.data());


	for (size_t i = 0; i < imageCount; ++i)
	{
		new (&swapChain->m_textures[i]) VulkanTexture2D(imageDesc, vkImages[i]);
	}

	/*VkImageViewCreateInfo imageViewCreateInfo{};
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = convertFormat_VT_to_VK(swapChain->getDesc().m_format);

	imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;*/

}

void VT_VK::VulkanGraphicDevice::createShaderInternal(const void* code, size_t codeSize, VkShaderModule& vkShaderModule)
{
	VkShaderModuleCreateInfo moduleCreateInfo{};
	moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	moduleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(code);
	moduleCreateInfo.codeSize = codeSize;

	vkCreateShaderModule(m_vkDevice, &moduleCreateInfo, nullptr, &vkShaderModule);
}

void VT_VK::VulkanGraphicDevice::destroyShaderInternal(VulkanShaderBase* shader)
{
	assert(shader);

	if (shader->m_vkShaderModule)
	{
		m_destroyingResources.m_shaderModules.addToContainer(shader->m_vkShaderModule);
	}
}

bool VT_VK::VulkanGraphicDevice::initDevice(bool isSwapChainEnabled)
{
	VkInstance vkInstance = getVulkanEnvironmentGraphicPlatform()->getInstance();

	VT_CHECK_INITIALIZATION(initVkDevice(vkInstance, isSwapChainEnabled));

	return true;
}

void VT_VK::VulkanGraphicDevice::releaseDevice()
{
	if (m_vkDevice)
	{
		wait();

		vkDestroyDevice(m_vkDevice, nullptr);
		m_vkDevice = nullptr;
	}
}

void VT_VK::VulkanGraphicDevice::update()
{
	destroyResources();
}

void VT_VK::VulkanGraphicDevice::wait()
{
	assert(m_vkDevice);
	vkDeviceWaitIdle(m_vkDevice);
}

bool VT_VK::VulkanGraphicDevice::createSwapChain(VT::ManagedGraphicDevice::ManagedSwapChainBase* swapChain,
	const VT::SwapChainDesc& desc, const VT::IWindow* window)
{
	VkSurfaceKHR vkSurface = 0;
	VkSwapchainKHR vkSwapChain = 0;

	VT::Texture2DDesc imageDesc{};

	createSwapChainInternal(desc, window, vkSurface, vkSwapChain, imageDesc);

	if (vkSurface == 0 || vkSwapChain == 0)
	{
		return false;
	}

	VulkanSwapChain* vulkanSwapChain = new (swapChain) VulkanSwapChain(desc, vkSwapChain, vkSurface);
	initSwapChainImages(vulkanSwapChain, imageDesc);

	return true;
}

void VT_VK::VulkanGraphicDevice::destroySwapChain(VT::ManagedGraphicDevice::ManagedSwapChainBase* swapChain)
{
	assert(swapChain);

	VulkanSwapChain* vulkanSwapChain = reinterpret_cast<VulkanSwapChain*>(swapChain);

	if (vulkanSwapChain->m_textures)
	{
		delete[] reinterpret_cast<uint8_t*>(vulkanSwapChain->m_textures);
	}

	if (vulkanSwapChain->m_vkSwapChain)
	{
		m_destroyingResources.m_swapChains.addToContainer(vulkanSwapChain->m_vkSwapChain);
	}

	if (vulkanSwapChain->m_vkSurface)
	{
		m_destroyingResources.m_surfaces.addToContainer(vulkanSwapChain->m_vkSurface);
	}
}

void VT_VK::VulkanGraphicDevice::destroyTexture2D(VT::ManagedGraphicDevice::ManagedTexture2DBase* texture)
{
	assert(texture);

	VulkanTexture2D* vulkanTexture = reinterpret_cast<VulkanTexture2D*>(texture);

	if (vulkanTexture->m_vkImage)
	{
		m_destroyingResources.m_images.addToContainer(vulkanTexture->m_vkImage);
	}
}

bool VT_VK::VulkanGraphicDevice::createVertexShader(VT::ManagedGraphicDevice::ManagedVertexShaderBase* shader,
	const void* code, size_t codeSize)
{
	VkShaderModule vkShaderModule;
	createShaderInternal(code, codeSize, vkShaderModule);

	VulkanVertexShader* vertexShader = new (shader) VulkanVertexShader(vkShaderModule);
	return vertexShader;
}

void VT_VK::VulkanGraphicDevice::destroyVertexShader(VT::ManagedGraphicDevice::ManagedVertexShaderBase* shader)
{
	destroyShaderInternal(reinterpret_cast<VulkanShaderBase*>(shader));
}

bool VT_VK::VulkanGraphicDevice::createPixelShader(VT::ManagedGraphicDevice::ManagedPixelShaderBase* shader,
	const void* code, size_t codeSize)
{
	VkShaderModule vkShaderModule;
	createShaderInternal(code, codeSize, vkShaderModule);

	VulkanPixelShader* pixelShader = new (shader) VulkanPixelShader(vkShaderModule);
	return pixelShader;
}

void VT_VK::VulkanGraphicDevice::destroyPixelShader(VT::ManagedGraphicDevice::ManagedPixelShaderBase* shader)
{
	destroyShaderInternal(reinterpret_cast<VulkanShaderBase*>(shader));
}

bool VT_VK::VulkanGraphicDevice::createPipelineState(VT::ManagedGraphicDevice::ManagedPipelineStateBase* state,
	const VT::PipelineStateInfo& info, const VT::IRenderPass* renderPass)
{
	VkPipelineVertexInputStateCreateInfo vertInputCreateInfo{};
	vertInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertInputCreateInfo.vertexBindingDescriptionCount = 0;
	vertInputCreateInfo.pVertexBindingDescriptions = nullptr;
	vertInputCreateInfo.vertexAttributeDescriptionCount = 0;
	vertInputCreateInfo.pVertexAttributeDescriptions = nullptr;

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo{};
	inputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyCreateInfo.primitiveRestartEnable = false;

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = 500.0f;
	viewport.height = 500.0f;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = { 500, 500 };

	VkPipelineViewportStateCreateInfo viewportCreateInfo{};
	viewportCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportCreateInfo.viewportCount = 1;
	viewportCreateInfo.pViewports = &viewport;
	viewportCreateInfo.scissorCount = 1;
	viewportCreateInfo.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizationCreateInfo{};
	rasterizationCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationCreateInfo.depthClampEnable = false;
	rasterizationCreateInfo.rasterizerDiscardEnable = false;
	rasterizationCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizationCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizationCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizationCreateInfo.depthBiasEnable = false;

	VkPipelineMultisampleStateCreateInfo multisampleCreateInfo{};
	multisampleCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleCreateInfo.sampleShadingEnable = false;
	multisampleCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT
		| VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = false;

	VkPipelineColorBlendStateCreateInfo blendCreateInfo{};
	blendCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	blendCreateInfo.logicOpEnable = false;
	blendCreateInfo.attachmentCount = 1;
	blendCreateInfo.pAttachments = &colorBlendAttachment;

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

	VkPipelineLayout vkPipelineLayout;
	if (!checkVkResult(
		vkCreatePipelineLayout(m_vkDevice, &pipelineLayoutCreateInfo, nullptr, &vkPipelineLayout)))
	{
		return false;
	}

	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
	if (info.m_vertexShader)
	{
		VulkanVertexShader* shader = reinterpret_cast<VulkanVertexShader*>(info.m_vertexShader);
		VkPipelineShaderStageCreateInfo stageCreateInfo{};
		stageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		stageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		stageCreateInfo.module = shader->getVkShaderModule();
		stageCreateInfo.pName = "VS";

		shaderStages.push_back(stageCreateInfo);
	}
	if (info.m_pixelShader)
	{
		VulkanPixelShader* shader = reinterpret_cast<VulkanPixelShader*>(info.m_pixelShader);
		VkPipelineShaderStageCreateInfo stageCreateInfo{};
		stageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		stageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		stageCreateInfo.module = shader->getVkShaderModule();
		stageCreateInfo.pName = "PS";

		shaderStages.push_back(stageCreateInfo);
	}

	const VulkanRenderPass* vulkanRenderPass = reinterpret_cast<const VulkanRenderPass*>(renderPass);

	VkGraphicsPipelineCreateInfo pipelineCreateInfo{};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.stageCount = shaderStages.size();
	pipelineCreateInfo.pStages = shaderStages.data();
	pipelineCreateInfo.pVertexInputState = &vertInputCreateInfo;
	pipelineCreateInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
	pipelineCreateInfo.pViewportState = &viewportCreateInfo;
	pipelineCreateInfo.pRasterizationState = &rasterizationCreateInfo;
	pipelineCreateInfo.pMultisampleState = &multisampleCreateInfo;
	pipelineCreateInfo.pColorBlendState = &blendCreateInfo;
	pipelineCreateInfo.layout = vkPipelineLayout;
	pipelineCreateInfo.renderPass = vulkanRenderPass->getVkRenderPass();

	VkPipeline vkPipeline;
	if (!vkCreateGraphicsPipelines(m_vkDevice, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &vkPipeline))
	{
		m_destroyingResources.m_pipelineLayouts.addToContainer(vkPipelineLayout);
		return false;
	}

	new (state) VulkanPipelineState(vkPipeline, vkPipelineLayout, info.getHash());

	return true;
}

void VT_VK::VulkanGraphicDevice::destroyPipelineState(VT::ManagedGraphicDevice::ManagedPipelineStateBase* state)
{
	assert(state);

	VulkanPipelineState* vulkanState = reinterpret_cast<VulkanPipelineState*>(state);
	if (vulkanState->m_vkPipeline)
	{
		m_destroyingResources.m_pipelines.addToContainer(vulkanState->m_vkPipeline);
	}

	if (vulkanState->m_vkPipelineLayout)
	{
		m_destroyingResources.m_pipelineLayouts.addToContainer(vulkanState->m_vkPipelineLayout);
	}
}

bool VT_VK::VulkanGraphicDevice::createRenderPass(VT::ManagedGraphicDevice::ManagedRenderPassBase* pass,
	const VT::RenderPassInfo& info)
{
	const VT::RenderPassInfo::AttachmentsContainer& attachments = info.m_attachments;
	const size_t attachmentsCount = attachments.size();

	std::vector<VkAttachmentDescription> attachmentDescs;
	for (uint32_t attachmentIndex = 0; attachmentIndex < attachmentsCount; ++attachmentIndex)
	{
		const VT::RenderPassAttachment& attachment = attachments[attachmentIndex];

		VkAttachmentDescription& desc = attachmentDescs.emplace_back();
		desc.format = convertFormat_VT_to_VK(attachment.m_format);
		desc.samples = VK_SAMPLE_COUNT_1_BIT;
		desc.loadOp = convertLoadingOperation_VT_to_VK(attachment.m_loadingOperation);
		desc.storeOp = convertStoringOperation_VT_to_VK(attachment.m_storingOperation);

		desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		desc.initialLayout = convertResourceLayout_VT_to_VK(attachment.m_initialLayout);
		desc.finalLayout = convertResourceLayout_VT_to_VK(attachment.m_targetLayout);
	}

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkRenderPassCreateInfo renderPassCreateInfo{};
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.attachmentCount = attachmentDescs.size();
	renderPassCreateInfo.pAttachments = attachmentDescs.data();
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpass;

	VkRenderPass vkRenderPass;
	if (!checkVkResult(vkCreateRenderPass(m_vkDevice, &renderPassCreateInfo, nullptr, &vkRenderPass)))
	{
		return false;
	}

	new (pass) VulkanRenderPass(vkRenderPass, info.getHash());

	return true;
}

void VT_VK::VulkanGraphicDevice::destroyRenderPass(VT::ManagedGraphicDevice::ManagedRenderPassBase* pass)
{
	assert(pass);

	VulkanRenderPass* vulkanPass = reinterpret_cast<VulkanRenderPass*>(pass);
	if (vulkanPass->m_vkRenderPass)
	{
		m_destroyingResources.m_renderPasses.addToContainer(vulkanPass->m_vkRenderPass);
	}
}

bool VT_VK::VulkanGraphicDevice::createCommandPool(VT::ManagedGraphicDevice::ManagedCommandPoolBase* commandPool)
{
	return false;
}

void VT_VK::VulkanGraphicDevice::destroyCommandPool(VT::ManagedGraphicDevice::ManagedCommandPoolBase* commandPool)
{}

VT::ManagedGraphicDevice::ManagedGraphicDevice::SwapChainStorage* VT_VK::VulkanGraphicDevice::createSwapChainStorage() const
{
	return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedSwapChainStorageInfo<VulkanSwapChain>>();
}

VT::ManagedGraphicDevice::ManagedGraphicDevice::Texture2DStorage* VT_VK::VulkanGraphicDevice::createTexture2DStorage() const
{
	return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedTexture2DStorageInfo<VulkanTexture2D>>();
}

VT::ManagedGraphicDevice::ManagedGraphicDevice::VertexShaderStorage* VT_VK::VulkanGraphicDevice::createVertexShaderStorage() const
{
	return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedVertexShaderStorageInfo<VulkanVertexShader>>();
}

VT::ManagedGraphicDevice::ManagedGraphicDevice::PixelShaderStorage* VT_VK::VulkanGraphicDevice::createPixelShaderStorage() const
{
	return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedPixelShaderStorageInfo<VulkanPixelShader>>();
}

VT::ManagedGraphicDevice::ManagedGraphicDevice::PipelineStateStorage* VT_VK::VulkanGraphicDevice::createPipelineStateStorage() const
{
	return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedPipelineStateStorageInfo<VulkanPipelineState>>();
}

VT::ManagedGraphicDevice::ManagedGraphicDevice::RenderPassStorage* VT_VK::VulkanGraphicDevice::createRenderPassStorage() const
{
	return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedRenderPassStorageInfo<VulkanRenderPass>>();
}

VT::ManagedGraphicDevice::ManagedGraphicDevice::CommandPoolStorage* VT_VK::VulkanGraphicDevice::createCommandPoolStorage() const
{
	return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedCommandPoolStorageInfo<VulkanCommandPool>>();
}
