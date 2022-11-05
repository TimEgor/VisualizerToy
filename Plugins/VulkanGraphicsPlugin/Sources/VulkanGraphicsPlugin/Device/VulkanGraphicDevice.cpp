#include "VulkanGraphicDevice.h"

#include "Core/UtilitiesMacros.h"
#include "Core/String/Format.h"

#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"
#include "Engine/EngineInstance.h"

#include "Platform/IPlatform.h"
#include "WindowSystem/IWindow.h"

#include "VulkanGraphicsPlugin/SwapChain/VulkanSwapChain.h"
#include "VulkanGraphicsPlugin/Utilities/FormatConverter.h"
#include "VulkanGraphicsPlugin/Utilities/PresentModeConverter.h"

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

	bool checkAvaliableInstanceLayersSupport(const VulkanNameContainer& layers)
	{
		bool result = true;

		uint32_t layerCount = 0;

		VkResult vkResult;
		vkResult = vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		result = checkVkResultReturnAssert(vkResult);

		if (result && layerCount > 0)
		{
			std::vector<VkLayerProperties> availableLayerProperties;
			availableLayerProperties.resize(layerCount);

			vkResult = vkEnumerateInstanceLayerProperties(&layerCount, availableLayerProperties.data());
			result = checkVkResultReturnAssert(vkResult);

			if (result)
			{
				for (const char* layerName : layers)
				{
					bool isLayerFound = false;

					for (const auto& availableLayerProperty : availableLayerProperties)
					{
						if (std::strncmp(layerName, availableLayerProperty.layerName, VK_MAX_EXTENSION_NAME_SIZE) == 0)
						{
							isLayerFound = true;
							break;
						}
					}

					if (!isLayerFound)
					{
						result = false;
						break;
					}
				}
			}
		}

		return result;
	}

	bool checkAvaliableInstanceExtensionsSupport(const VulkanNameContainer& extensions)
	{
		bool result = true;

		uint32_t extensionCount = 0;

		VkResult vkResult;
		vkResult = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		result = checkVkResultReturnAssert(vkResult);

		if (result && extensionCount > 0)
		{
			std::vector<VkExtensionProperties> availableExtensionProperties;
			availableExtensionProperties.resize(extensionCount);

			vkResult = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensionProperties.data());
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

	bool checkAvaliableDeviceExtensionsSupport(VkPhysicalDevice device, const VulkanNameContainer& extensions)
	{
		bool result = true;

		uint32_t extensionCount = 0;

		VkResult vkResult;
		vkResult = vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
		result = checkVkResultReturnAssert(vkResult);

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

bool VT_VK::VulkanGraphicDevice::initVkInstance(bool swapChainEnabled)
{
	VkApplicationInfo applicationInfo{};
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pApplicationName = "VT_TEST_APP";
	applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
	applicationInfo.pEngineName = "VT";
	applicationInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
	applicationInfo.apiVersion = VK_API_VERSION_1_3;

	VkInstanceCreateInfo instanceCreateInfo{};

	VulkanNameContainer extensionNames;
	if (swapChainEnabled)
	{
		extensionNames.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

#ifdef WIN32
		extensionNames.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif
	}

	if (!extensionNames.empty())
	{
		VT_CHECK_RETURN_FALSE_ASSERT(checkAvaliableInstanceExtensionsSupport(extensionNames));

		instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensionNames.size());
		instanceCreateInfo.ppEnabledExtensionNames = extensionNames.data();
	}

	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;

#ifdef _DEBUG
	const VulkanNameContainer validationLayers = { "VK_LAYER_KHRONOS_validation" };
	VT_CHECK_RETURN_FALSE(checkAvaliableInstanceLayersSupport(validationLayers));

	instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
#endif

	VkResult creationResult = vkCreateInstance(&instanceCreateInfo, nullptr, &m_vkInstance);

	return checkVkResultReturnAssert(creationResult);
}

bool VT_VK::VulkanGraphicDevice::initVkDevice(bool swapChainEnabled)
{
	VulkanNameContainer extensionNames;

	if (swapChainEnabled)
	{
		extensionNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	}

	VT_CHECK_RETURN_FALSE(chooseVkPhysDevice(extensionNames));

	findQueueFamiliesIndices();

	VT_CHECK_RETURN_FALSE(m_graphicQueueFamilyIndex != VK_QUEUE_FAMILY_IGNORED
		&& m_transferQueueFamilyIndex != VK_QUEUE_FAMILY_IGNORED
		&& m_computeQueueFamilyIndex != VK_QUEUE_FAMILY_IGNORED);

	std::vector<VulkanQueueFamilyIndex> requiringFamiliesQueue = { m_graphicQueueFamilyIndex, m_transferQueueFamilyIndex, m_computeQueueFamilyIndex };
	std::sort(requiringFamiliesQueue.begin(), requiringFamiliesQueue.end());
	requiringFamiliesQueue.erase(std::unique(requiringFamiliesQueue.begin(), requiringFamiliesQueue.end()), requiringFamiliesQueue.end());
	uint32_t familyCount = static_cast<uint32_t>(requiringFamiliesQueue.size());

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfo(familyCount);

	float queuePriority = 1.0f;
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

bool VT_VK::VulkanGraphicDevice::chooseVkPhysDevice(const VulkanNameContainer& extensions)
{
	assert(!m_vkPhysDevice);

	VulkanPhysDevicesContainer physDevices;
	VT_CHECK_RETURN_FALSE(enumeratePhysDevices(m_vkInstance, physDevices) && physDevices.size() > 0);

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

void VT_VK::VulkanGraphicDevice::createSwapChainInternal(const VT::SwapChainDesc& desc, VT::IWindow* window, VkSurfaceKHR& surface, VkSwapchainKHR& swapChain)
{
	VT::IPlatform* platform = VT::EngineInstance::getInstance()->getEnvironment()->m_platform;

	VT_CHECK_RETURN_ASSERT(window && platform);

	surface = 0;
	swapChain = 0;

#ifdef WIN32
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo{};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.hwnd = reinterpret_cast<HWND>(window->getNativeHandle());
	surfaceCreateInfo.hinstance = reinterpret_cast<HINSTANCE>(platform->getNativeHandle());

	vkCreateWin32SurfaceKHR(m_vkInstance, &surfaceCreateInfo, nullptr, &surface);
#endif

	VT_CHECK_RETURN_ASSERT(surface);

	VulkanSwapChainInfo capabilitiesInfo;
	getSwapChainCapabilitiesInfo(surface, capabilitiesInfo);

	bool checkProperty = false;
	VkFormat requiringVkFormat = convertFormat_VT_to_VK(desc.m_format);
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
	VkPresentModeKHR requiringVkPresentMode = convertPresentMode_VT_to_VK(desc.m_presentMode);
	for (VkPresentModeKHR availableMode : capabilitiesInfo.m_presentModes)
	{
		if (availableMode == requiringVkPresentMode)
		{
			checkProperty = true;
			break;
		}
	}

	VT_CHECK_RETURN_ASSERT_MSG(checkProperty, VT::stringFormat("Window surface doesn't support a requiring swap chain present mode (%d).", desc.m_presentMode).c_str());

	VT_CHECK_RETURN_ASSERT_MSG(capabilitiesInfo.m_capabilities.minImageCount <= desc.m_imageCount
		&& desc.m_imageCount <= capabilitiesInfo.m_capabilities.minImageCount,
		VT::stringFormat("Window surface doesn't support a requiring swap chain image count (%d).", desc.m_imageCount).c_str());

	VkSwapchainCreateInfoKHR swapChainCreateInfo{};
	swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapChainCreateInfo.surface = surface;
	swapChainCreateInfo.minImageCount = desc.m_imageCount;
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
}

bool VT_VK::VulkanGraphicDevice::init(bool swapChainEnabled)
{
	VT_CHECK_INITIALIZATION(initVkInstance(swapChainEnabled));

	volkLoadInstance(m_vkInstance);

	VT_CHECK_INITIALIZATION(initVkDevice(swapChainEnabled));

	return true;
}

void VT_VK::VulkanGraphicDevice::release()
{
	if (m_vkDevice)
	{
		wait();

		vkDestroyDevice(m_vkDevice, nullptr);
		m_vkDevice = nullptr;
	}

	if (m_vkInstance)
	{
		vkDestroyInstance(m_vkInstance, nullptr);
		m_vkInstance = nullptr;
	}
}

void VT_VK::VulkanGraphicDevice::wait()
{
	assert(m_vkDevice);
	vkDeviceWaitIdle(m_vkDevice);
}

VT::ISwapChain* VT_VK::VulkanGraphicDevice::createSwapChain(const VT::SwapChainDesc& desc, VT::IWindow* window)
{
	VkSurfaceKHR surface = 0;
	VkSwapchainKHR swapChain = 0;

	createSwapChainInternal(desc, window, surface, swapChain);

	if (surface == 0 || swapChain == 0)
	{
		return nullptr;
	}

	return new VulkanSwapChain(swapChain, surface, desc);
}

bool VT_VK::VulkanGraphicDevice::createSwapChain(const VT::SwapChainDesc& desc, VT::IWindow* window, void* swapChainPtr)
{
	VkSurfaceKHR surface = 0;
	VkSwapchainKHR swapChain = 0;

	createSwapChainInternal(desc, window, surface, swapChain);

	if (surface == 0 || swapChain == 0)
	{
		return false;
	}

	new (reinterpret_cast<VulkanSwapChain*>(swapChainPtr)) VulkanSwapChain(swapChain, surface, desc);
	return true;
}
