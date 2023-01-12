#include "VulkanGraphicDevice.h"

#include "Core/UtilitiesMacros.h"
#include "Core/String/Format.h"

#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"
#include "Engine/EngineInstance.h"

#include "Platform/IPlatform.h"
#include "WindowSystem/IWindow.h"

#include "VulkanGraphicsPlugin/SwapChain/VulkanSwapChain.h"
#include "VulkanGraphicsPlugin/Buffer/VulkanGPUBuffer.h"
#include "VulkanGraphicsPlugin/Commands/VulkanCommandPool.h"
#include "VulkanGraphicsPlugin/Shaders/VulkanShaders.h"
#include "VulkanGraphicsPlugin/Pipeline/VulkanPipelineState.h"
#include "VulkanGraphicsPlugin/Pipeline/VulkanPipelineBindingLayout.h"
#include "VulkanGraphicsPlugin/Synchronization/VulkanFence.h"
#include "VulkanGraphicsPlugin/Synchronization/VulkanSemaphore.h"

#include "VulkanGraphicsPlugin/Utilities/FormatConverter.h"
#include "VulkanGraphicsPlugin/Utilities/PresentModeConverter.h"
#include "VulkanGraphicsPlugin/Utilities/ImageAspectConverter.h"
#include "VulkanGraphicsPlugin/Utilities/BufferUsageConverter.h"
#include "VulkanGraphicsPlugin/Utilities/InputLayoutConverter.h"

#include "VulkanGraphicsPlugin/Utilities/VulkanEnvironmentGraphicPlatform.h"

#include <algorithm>
#include <unordered_set>

#include "InputLayout/InputLayout.h"


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

	extensionNames.push_back(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);

	if (isSwapChainEnabled)
	{
		extensionNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	}


	VT_CHECK_RETURN_FALSE(chooseVkPhysDevice(vkInstance, extensionNames));

	vkGetPhysicalDeviceMemoryProperties(m_vkPhysDevice, &m_physMemoryProps);

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

	VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamicRenderingFeature{};
	dynamicRenderingFeature.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
	dynamicRenderingFeature.dynamicRendering = true;

	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pNext = &dynamicRenderingFeature;
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

uint32_t VT_VK::VulkanGraphicDevice::chooseMemoryType(uint32_t filterBits, VkMemoryPropertyFlags properties)
{
	for (uint32_t typeIndex = 0; typeIndex < m_physMemoryProps.memoryTypeCount; ++typeIndex)
	{
		if ((filterBits & (1 << typeIndex)) &&
			(properties & m_physMemoryProps.memoryTypes[typeIndex].propertyFlags) == properties)
		{
			return typeIndex;
		}
	}
}

void VT_VK::VulkanGraphicDevice::destroyResources()
{
	wait();

	VkInstance vkInstance = getVulkanEnvironmentGraphicPlatform()->getInstance();

	m_destroyingResources.destroyResources(vkInstance, m_vkDevice);
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

	checkVkResultAssert(vkCreateWin32SurfaceKHR(vkInstance, &surfaceCreateInfo, nullptr, &surface));
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
	swapChain->m_targetViews = reinterpret_cast<VulkanTexture2DView*>(new uint8_t[sizeof(VulkanTexture2DView) * imageCount]);
	swapChain->m_textureCount = imageCount;

	vkGetSwapchainImagesKHR(m_vkDevice, swapChain->m_vkSwapChain, &imageCount, vkImages.data());

	VT::TextureViewDesc targetViewDesc{};
	targetViewDesc.m_aspect = VT::TextureViewAspect::COLOR;
	targetViewDesc.m_baseMipLevel = 0;
	targetViewDesc.m_mipLevelCount = 1;
	targetViewDesc.m_baseArrayLayer = 0;
	targetViewDesc.m_arrayLayerCount = 1;

	for (size_t i = 0; i < imageCount; ++i)
	{
		VulkanTexture2D* targetTexture = new (&swapChain->m_textures[i]) VulkanTexture2D(imageDesc, vkImages[i]);
		createTexture2DView(&swapChain->m_targetViews[i], targetTexture, targetViewDesc);
	}
}

void VT_VK::VulkanGraphicDevice::createShaderInternal(const void* code, size_t codeSize, VkShaderModule& vkShaderModule)
{
	VkShaderModuleCreateInfo moduleCreateInfo{};
	moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	moduleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(code);
	moduleCreateInfo.codeSize = codeSize;

	checkVkResultAssert(vkCreateShaderModule(m_vkDevice, &moduleCreateInfo, nullptr, &vkShaderModule));
}

void VT_VK::VulkanGraphicDevice::destroyShaderInternal(VulkanShaderBase* shader)
{
	assert(shader);

	if (shader->m_vkShaderModule)
	{
		m_destroyingResources.m_shaderModules.addToContainer(shader->m_vkShaderModule);
	}
}

void VT_VK::VulkanGraphicDevice::createSemaphoreInternal(VkSemaphore& vkSemaphore)
{
	VkSemaphoreCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	if (!checkVkResult(vkCreateSemaphore(m_vkDevice, &createInfo, nullptr, &vkSemaphore)))
	{
		vkSemaphore = 0;
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
	destroyResources();

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

	VkSemaphore vkTextureAvailableSemaphore = 0;
	createSemaphoreInternal(vkTextureAvailableSemaphore);
	if (vkTextureAvailableSemaphore == 0)
	{
		return false;
	}

	VulkanSwapChain* vulkanSwapChain = new (swapChain) VulkanSwapChain(desc, m_vkDevice,
		vkSwapChain, vkSurface, m_vkGraphicQueue, vkTextureAvailableSemaphore);
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

	if (vulkanSwapChain->m_targetViews)
	{
		for (uint32_t i = 0; i < vulkanSwapChain->getTextureCount(); ++i)
		{
			destroyTexture2DView(&vulkanSwapChain->m_targetViews[i]);
		}

		delete[] reinterpret_cast<uint8_t*>(vulkanSwapChain->m_targetViews);
	}

	destroySemaphore(&vulkanSwapChain->m_textureAvailableSemaphore);

	if (vulkanSwapChain->m_vkSwapChain)
	{
		m_destroyingResources.m_swapChains.addToContainer(vulkanSwapChain->m_vkSwapChain);
	}

	if (vulkanSwapChain->m_vkSurface)
	{
		m_destroyingResources.m_surfaces.addToContainer(vulkanSwapChain->m_vkSurface);
	}
}

bool VT_VK::VulkanGraphicDevice::createBuffer(VT::ManagedGraphicDevice::ManagedGPUBufferBase* buffer, const VT::GPUBufferDesc& desc)
{
	VkBufferCreateInfo bufferCreateInfo{};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = desc.m_byteSize;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferCreateInfo.usage = converteBufferUsageVTtoVK(desc.m_usage);

	VkBuffer vkBuffer = 0;
	if (!checkVkResult(vkCreateBuffer(m_vkDevice, &bufferCreateInfo, nullptr, &vkBuffer)))
	{
		return false;
	}

	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(m_vkDevice, vkBuffer, &memoryRequirements);

	VkMemoryAllocateInfo allocateInfo{};
	allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocateInfo.allocationSize = memoryRequirements.size;
	allocateInfo.memoryTypeIndex = chooseMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	VkDeviceMemory vkMemory = 0;
	if (!checkVkResult(vkAllocateMemory(m_vkDevice, &allocateInfo, nullptr, &vkMemory)))
	{
		m_destroyingResources.m_buffers.addToContainer(vkBuffer);

		return false;
	}

	vkBindBufferMemory(m_vkDevice, vkBuffer, vkMemory, 0);

	new (buffer) VulkanGPUBuffer(desc, m_vkDevice, vkBuffer, vkMemory);

	return true;
}

void VT_VK::VulkanGraphicDevice::destroyBuffer(VT::ManagedGraphicDevice::ManagedGPUBufferBase* buffer)
{
	assert(buffer);

	VulkanGPUBuffer* vulkanGpuBuffer = reinterpret_cast<VulkanGPUBuffer*>(buffer);

	if (vulkanGpuBuffer->m_vkMemory)
	{
		vkFreeMemory(m_vkDevice, vulkanGpuBuffer->m_vkMemory, nullptr);
	}

	if (vulkanGpuBuffer->m_vkBuffer)
	{
		m_destroyingResources.m_buffers.addToContainer(vulkanGpuBuffer->m_vkBuffer);
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

bool VT_VK::VulkanGraphicDevice::createTexture2DView(VT::ManagedGraphicDevice::ManagedTexture2DViewBase* view, VT::ITexture2D* texture, const VT::TextureViewDesc& desc)
{
	assert(texture);

	VulkanTexture2D* vulkanTexture = reinterpret_cast<VulkanTexture2D*>(texture);

	VkImageViewCreateInfo imageViewCreateInfo{};
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = convertFormat_VT_to_VK(vulkanTexture->m_desc.m_format);
	imageViewCreateInfo.image = vulkanTexture->m_vkImage;

	imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	imageViewCreateInfo.subresourceRange.aspectMask = convertImageAspect_VT_to_VK(desc.m_aspect);
	imageViewCreateInfo.subresourceRange.baseMipLevel = desc.m_baseMipLevel;
	imageViewCreateInfo.subresourceRange.levelCount = desc.m_mipLevelCount;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = desc.m_baseArrayLayer;
	imageViewCreateInfo.subresourceRange.layerCount = desc.m_arrayLayerCount;

	VkImageView vkImageView = 0;
	if (!checkVkResult(vkCreateImageView(m_vkDevice, &imageViewCreateInfo, nullptr, &vkImageView)))
	{
		return false;
	}

	new (view) VulkanTexture2DView(desc, vkImageView);

	return true;
}

void VT_VK::VulkanGraphicDevice::destroyTexture2DView(VT::ManagedGraphicDevice::ManagedTexture2DViewBase* view)
{
	assert(view);

	VulkanTexture2DView* vulkanView = reinterpret_cast<VulkanTexture2DView*>(view);
	if(vulkanView->m_vkImageView)
	{
		m_destroyingResources.m_imageViews.addToContainer(vulkanView->m_vkImageView);
	}
}

bool VT_VK::VulkanGraphicDevice::createVertexShader(VT::ManagedGraphicDevice::ManagedVertexShaderBase* shader,
	const void* code, size_t codeSize)
{
	VkShaderModule vkShaderModule;
	createShaderInternal(code, codeSize, vkShaderModule);

	new (shader) VulkanVertexShader(vkShaderModule);

	return true;
}

void VT_VK::VulkanGraphicDevice::destroyVertexShader(VT::ManagedGraphicDevice::ManagedVertexShaderBase* shader)
{
	destroyShaderInternal(reinterpret_cast<VulkanVertexShader*>(shader));
}

bool VT_VK::VulkanGraphicDevice::createPixelShader(VT::ManagedGraphicDevice::ManagedPixelShaderBase* shader,
	const void* code, size_t codeSize)
{
	VkShaderModule vkShaderModule;
	createShaderInternal(code, codeSize, vkShaderModule);

	new (shader) VulkanPixelShader(vkShaderModule);

	return true;
}

void VT_VK::VulkanGraphicDevice::destroyPixelShader(VT::ManagedGraphicDevice::ManagedPixelShaderBase* shader)
{
	destroyShaderInternal(reinterpret_cast<VulkanPixelShader*>(shader));
}

bool VT_VK::VulkanGraphicDevice::createPipelineState(VT::ManagedGraphicDevice::ManagedPipelineStateBase* state,
	const VT::PipelineStateInfo& info, const VT::IPipelineBindingLayout* bindingLayout, const VT::InputLayoutDesc* inputLayoutDesc)
{
	assert(bindingLayout);

	VkPipelineVertexInputStateCreateInfo vertInputCreateInfo{};
	vertInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	std::vector<VkVertexInputBindingDescription> vkBindings;
	std::vector<VkVertexInputAttributeDescription> vkAttribs;
	if (inputLayoutDesc)
	{
		vkBindings.reserve(inputLayoutDesc->m_bindings.size());
		for (auto& binding : inputLayoutDesc->m_bindings)
		{
			VkVertexInputBindingDescription& vkBinding = vkBindings.emplace_back();
			vkBinding.binding = binding.m_index;
			vkBinding.stride = binding.m_stride;
			vkBinding.inputRate = converteInputLayoutBindingVTtoVK(binding.m_type);
		}

		vkAttribs.reserve(inputLayoutDesc->m_elements.size());
		for (auto& element : inputLayoutDesc->m_elements)
		{
			VkVertexInputAttributeDescription& vkAttrib = vkAttribs.emplace_back();
			vkAttrib.binding = element.m_slot;
			vkAttrib.location = element.m_index;
			vkAttrib.offset = element.m_offset;
			vkAttrib.format = convertInputLayoutFormatVTtoVK(element.m_type, element.m_componentNum);
		}

		vertInputCreateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(vkBindings.size());
		vertInputCreateInfo.pVertexBindingDescriptions = vkBindings.data();
		vertInputCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vkAttribs.size());
		vertInputCreateInfo.pVertexAttributeDescriptions = vkAttribs.data();
	}

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
	rasterizationCreateInfo.lineWidth = 1.0f;

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

	std::vector<VkFormat> vkFormats;
	vkFormats.reserve(info.m_formats.size());

	for (VT::Format format : info.m_formats)
	{
		vkFormats.push_back(convertFormat_VT_to_VK(format));
	}

	VkPipelineRenderingCreateInfoKHR pipelineRenderingCreateInfo{};
	pipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
	pipelineRenderingCreateInfo.colorAttachmentCount = vkFormats.size();
	pipelineRenderingCreateInfo.pColorAttachmentFormats = vkFormats.data();

	const VulkanPipelineBindingLayout* vulkanBindingLayout = reinterpret_cast<const VulkanPipelineBindingLayout*>(bindingLayout);

	VkGraphicsPipelineCreateInfo pipelineCreateInfo{};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.pNext = &pipelineRenderingCreateInfo;
	pipelineCreateInfo.stageCount = shaderStages.size();
	pipelineCreateInfo.pStages = shaderStages.data();
	pipelineCreateInfo.pVertexInputState = &vertInputCreateInfo;
	pipelineCreateInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
	pipelineCreateInfo.pViewportState = &viewportCreateInfo;
	pipelineCreateInfo.pRasterizationState = &rasterizationCreateInfo;
	pipelineCreateInfo.pMultisampleState = &multisampleCreateInfo;
	pipelineCreateInfo.pColorBlendState = &blendCreateInfo;
	pipelineCreateInfo.layout = vulkanBindingLayout->getVkPipelineLayout();
	pipelineCreateInfo.renderPass = VK_NULL_HANDLE;

	VkPipeline vkPipeline;
	if (!checkVkResult(vkCreateGraphicsPipelines(m_vkDevice, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &vkPipeline)))
	{
		return false;
	}

	new (state) VulkanPipelineState(vkPipeline, info.getHash());

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
}

bool VT_VK::VulkanGraphicDevice::createPipelineBindingLayout(
	VT::ManagedGraphicDevice::ManagedPipelineBindingLayoutBase* layout, const VT::PipelineBindingLayoutDesc& desc)
{
	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

	std::vector<VkPushConstantRange> constantRanges;

	for (const auto& constBinding : desc.m_constDescriptorBindings)
	{
		VkPushConstantRange& constRange = constantRanges.emplace_back();
		constRange.size = constBinding.m_valuesCount;
	}

	VkPipelineLayout vkPipelineLayout;
	if (!checkVkResult(
		vkCreatePipelineLayout(m_vkDevice, &pipelineLayoutCreateInfo, nullptr, &vkPipelineLayout)))
	{
		return false;
	}

	VT::PipelineBindingLayoutHash hash = desc.getHash();

	new (layout) VulkanPipelineBindingLayout(vkPipelineLayout, hash);

	return true;
}

void VT_VK::VulkanGraphicDevice::destroyPipelineBindingLayout(VT::ManagedGraphicDevice::ManagedPipelineBindingLayoutBase* layout)
{
	VulkanPipelineBindingLayout* vulkanPipeline = reinterpret_cast<VulkanPipelineBindingLayout*>(layout);

	if (vulkanPipeline->m_vkPipelineLayout)
	{
		m_destroyingResources.m_pipelineLayouts.addToContainer(vulkanPipeline->m_vkPipelineLayout);
	}
}

bool VT_VK::VulkanGraphicDevice::createCommandPool(VT::ManagedGraphicDevice::ManagedCommandPoolBase* commandPool)
{
	VkCommandPoolCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	createInfo.queueFamilyIndex = m_graphicQueueFamilyIndex;
	createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	VkCommandPool vkCommandPool = 0;
	if (!checkVkResult(vkCreateCommandPool(m_vkDevice, &createInfo, nullptr, &vkCommandPool)))
	{
		return false;
	}

	new (commandPool) VulkanCommandPool(vkCommandPool);

	return true;
}

void VT_VK::VulkanGraphicDevice::destroyCommandPool(VT::ManagedGraphicDevice::ManagedCommandPoolBase* commandPool)
{
	assert(commandPool);

	VulkanCommandPool* vulkanCommandPool = reinterpret_cast<VulkanCommandPool*>(commandPool);
	if (vulkanCommandPool->m_vkCommandPool)
	{
		m_destroyingResources.m_commandPools.addToContainer(vulkanCommandPool->m_vkCommandPool);
	}
}

bool VT_VK::VulkanGraphicDevice::allocateCommandList(VT::ManagedGraphicDevice::ManagedCommandListBase* commandList, VT::ICommandPool* pool)
{
	assert(pool);

	VulkanCommandPool* vulkanPool = reinterpret_cast<VulkanCommandPool*>(pool);

	VkCommandBufferAllocateInfo allocateInfo{};
	allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocateInfo.commandPool = vulkanPool->m_vkCommandPool;
	allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocateInfo.commandBufferCount = 1;

	VkCommandBuffer vkBuffer = 0;
	if (!checkVkResult(vkAllocateCommandBuffers(m_vkDevice, &allocateInfo, &vkBuffer)))
	{
		return false;
	}

	new (commandList) VulkanCommandList(vkBuffer);

	return true;
}

void VT_VK::VulkanGraphicDevice::submitCommandList(VT::ICommandList* list, const VT::CommandListSubmitInfo& info)
{
	VulkanCommandList* vulkanCommandList = reinterpret_cast<VulkanCommandList*>(list);

	VkSemaphore waitSemaphore = info.m_waitSemaphore ?
		reinterpret_cast<VulkanSemaphore*>(info.m_waitSemaphore)->m_vkSemaphore :
		VK_NULL_HANDLE;
	
	VkSemaphore completeSemaphore = info.m_completeSemaphore ?
		reinterpret_cast<VulkanSemaphore*>(info.m_completeSemaphore)->m_vkSemaphore :
		VK_NULL_HANDLE;

	VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	VkFence vkFence = info.m_fence ?
		reinterpret_cast<VulkanFence*>(info.m_fence)->m_vkFence :
		VK_NULL_HANDLE;

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &vulkanCommandList->m_vkCommandBuffer;
	submitInfo.waitSemaphoreCount = waitSemaphore != VK_NULL_HANDLE ? 1 : 0;
	submitInfo.pWaitSemaphores = &waitSemaphore;
	submitInfo.pWaitDstStageMask = &waitStages;
	submitInfo.signalSemaphoreCount = completeSemaphore != VK_NULL_HANDLE ? 1 : 0;
	submitInfo.pSignalSemaphores = &completeSemaphore;

	checkVkResultAssert(vkQueueSubmit(m_vkGraphicQueue, 1, &submitInfo, vkFence));
}

bool VT_VK::VulkanGraphicDevice::createFence(VT::ManagedGraphicDevice::ManagedFenceBase* fence, bool signaled)
{
	VkFenceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	createInfo.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

	VkFence vkFence = 0;
	if (!checkVkResult(vkCreateFence(m_vkDevice, &createInfo, nullptr, &vkFence)))
	{
		return false;
	}

	new (fence) VulkanFence(vkFence);

	return true;
}

void VT_VK::VulkanGraphicDevice::destroyFence(VT::ManagedGraphicDevice::ManagedFenceBase* fence)
{
	assert(fence);

	VulkanFence* vulkanFence = reinterpret_cast<VulkanFence*>(fence);
	if (vulkanFence->m_vkFence)
	{
		m_destroyingResources.m_fences.addToContainer(vulkanFence->m_vkFence);
	}
}

void VT_VK::VulkanGraphicDevice::waitFences(uint32_t count, VT::IFence* fences)
{
	std::vector<VkFence> vkFences;
	vkFences.reserve(count);

	for (size_t i = 0; i < count; ++i)
	{
		VulkanFence* vulkanFence = reinterpret_cast<VulkanFence*>(&fences[i]);
		vkFences.push_back(vulkanFence->m_vkFence);
	}

	vkWaitForFences(m_vkDevice, vkFences.size(), vkFences.data(), true, UINT64_MAX);
}

void VT_VK::VulkanGraphicDevice::resetFences(uint32_t count, VT::IFence* fences)
{
	std::vector<VkFence> vkFences;
	vkFences.reserve(count);

	for (size_t i = 0; i < count; ++i)
	{
		VulkanFence* vulkanFence = reinterpret_cast<VulkanFence*>(&fences[i]);
		vkFences.push_back(vulkanFence->m_vkFence);
	}

	vkResetFences(m_vkDevice, vkFences.size(), vkFences.data());
}

bool VT_VK::VulkanGraphicDevice::createSemaphore(VT::ManagedGraphicDevice::ManagedSemaphoreBase* semaphore)
{
	VkSemaphore vkSemaphore = 0;
	createSemaphoreInternal(vkSemaphore);
	if (vkSemaphore == 0)
	{
		return false;
	}

	new (semaphore) VulkanSemaphore(vkSemaphore);

	return true;
}

void VT_VK::VulkanGraphicDevice::destroySemaphore(VT::ManagedGraphicDevice::ManagedSemaphoreBase* semaphore)
{
	assert(semaphore);

	VulkanSemaphore* vulkanSemaphore = reinterpret_cast<VulkanSemaphore*>(semaphore);
	if (vulkanSemaphore->m_vkSemaphore)
	{
		m_destroyingResources.m_semaphores.addToContainer(vulkanSemaphore->m_vkSemaphore);
	}
}

VT::ManagedGraphicDevice::ManagedGraphicDevice::SwapChainStorage* VT_VK::VulkanGraphicDevice::createSwapChainStorage() const
{
	return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedSwapChainStorageInfo<VulkanSwapChain>>();
}

VT::ManagedGraphicDevice::ManagedGraphicDevice::BufferStorage* VT_VK::VulkanGraphicDevice::createBufferStorage() const
{
	return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedGPUBufferStorageInfo<VulkanGPUBuffer>>();
}

VT::ManagedGraphicDevice::ManagedGraphicDevice::Texture2DStorage* VT_VK::VulkanGraphicDevice::createTexture2DStorage() const
{
	return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedTexture2DStorageInfo<VulkanTexture2D>>();
}

VT::ManagedGraphicDevice::ManagedGraphicDevice::Texture2DViewStorage* VT_VK::VulkanGraphicDevice::createTexture2DViewStorage() const
{
	return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedTexture2DViewStorageInfo<VulkanTexture2DView>>();
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

VT::ManagedGraphicDevice::ManagedGraphicDevice::PipelineBindingLayoutStorage* VT_VK::VulkanGraphicDevice::createPipelineBindingLayoutStorage() const
{
	return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedPipelineBindingLayoutStorageInfo<VulkanPipelineBindingLayout>>;
}

VT::ManagedGraphicDevice::ManagedGraphicDevice::CommandPoolStorage* VT_VK::VulkanGraphicDevice::createCommandPoolStorage() const
{
	return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedCommandPoolStorageInfo<VulkanCommandPool>>();
}

VT::ManagedGraphicDevice::ManagedGraphicDevice::CommandListStorage* VT_VK::VulkanGraphicDevice::createCommandListStorage() const
{
	return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedCommandListStorageInfo<VulkanCommandList>>();
}

VT::ManagedGraphicDevice::ManagedGraphicDevice::FenceStorage* VT_VK::VulkanGraphicDevice::createFenceStorage() const
{
	return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedFenceStorageInfo<VulkanFence>>();
}

VT::ManagedGraphicDevice::ManagedGraphicDevice::SemaphoreStorage* VT_VK::VulkanGraphicDevice::createSemaphoreStorage() const
{
	return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedSemaphoreStorageInfo<VulkanSemaphore>>();
}