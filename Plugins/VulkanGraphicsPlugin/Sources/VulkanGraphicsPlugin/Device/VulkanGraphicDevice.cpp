#include "VulkanGraphicDevice.h"

#include "Core/UtilitiesMacros.h"

#include <set>

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

	bool checkAvaliableInstanceLayersSupport(const VulkanLayerNameContainer& layers)
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
						if (std::strncmp(layerName, availableLayerProperty.layerName, 256) == 0)
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
}

bool VT_VK::VulkanGraphicDevice::initVkInstance()
{
	VkApplicationInfo applicationInfo{};
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pApplicationName = "VT_TEST_APP";
	applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
	applicationInfo.pEngineName = "VT";
	applicationInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
	applicationInfo.apiVersion = VK_API_VERSION_1_3;

	VkInstanceCreateInfo instanceCreateInfo{};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;

#ifdef _DEBUG
	const VulkanLayerNameContainer validationLayers = { "VK_LAYER_KHRONOS_validation" };
	VT_CHECK_RETURN_FALSE(checkAvaliableInstanceLayersSupport(validationLayers));

	instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
#endif

	VkResult creationResult = vkCreateInstance(&instanceCreateInfo, nullptr, &m_vkInstance);

	return checkVkResultReturnAssert(creationResult);
}

bool VT_VK::VulkanGraphicDevice::initVkDevice()
{
	VT_CHECK_RETURN_FALSE(chooseVkPhysDevice());

	findQueueFamiliesIndices();

	VT_CHECK_RETURN_FALSE(m_graphicQueueFamilyIndex != VK_QUEUE_FAMILY_IGNORED
		&& m_transferQueueFamilyIndex != VK_QUEUE_FAMILY_IGNORED
		&& m_computeQueueFamilyIndex != VK_QUEUE_FAMILY_IGNORED);

	std::set<VulkanQueueFamilyIndex> requiringFamiliesQueueSet = { m_graphicQueueFamilyIndex, m_transferQueueFamilyIndex, m_computeQueueFamilyIndex };
	std::vector<VulkanQueueFamilyIndex> requiringFamiliesQueue = { requiringFamiliesQueueSet.begin(), requiringFamiliesQueueSet .end()};
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

	VkResult creationResult = vkCreateDevice(m_vkPhysDevice, &deviceCreateInfo, nullptr, &m_vkDevice);
	VT_CHECK_RETURN_FALSE(checkVkResult(creationResult));

	vkGetDeviceQueue(m_vkDevice, m_graphicQueueFamilyIndex, 0, &m_vkGraphicQueue);
	vkGetDeviceQueue(m_vkDevice, m_transferQueueFamilyIndex, 0, &m_vkTransferQueue);
	vkGetDeviceQueue(m_vkDevice, m_computeQueueFamilyIndex, 0, &m_vkComputeQueue);

	VT_CHECK_RETURN_FALSE(m_vkGraphicQueue && m_vkTransferQueue && m_vkComputeQueue);

	return true;
}

bool VT_VK::VulkanGraphicDevice::chooseVkPhysDevice()
{
	assert(!m_vkPhysDevice);

	VulkanPhysDevicesContainer physDevices;
	VT_CHECK_RETURN_FALSE(enumeratePhysDevices(m_vkInstance, physDevices) && physDevices.size() > 0);

	for (auto device : physDevices)
	{
		if (checkVkPhysDevice(device))
		{
			m_vkPhysDevice = device;
			break;
		}
	}

	return m_vkPhysDevice;
}

bool VT_VK::VulkanGraphicDevice::checkVkPhysDevice(VkPhysicalDevice device)
{
	//VkPhysicalDeviceProperties properties;
	//vkGetPhysicalDeviceProperties(device, &properties);

	//VkPhysicalDeviceFeatures features;
	//vkGetPhysicalDeviceFeatures(device, &features);

	return true;
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

bool VT_VK::VulkanGraphicDevice::init()
{
	VT_CHECK_INITIALIZATION(initVkInstance());

	volkLoadInstance(m_vkInstance);

	VT_CHECK_INITIALIZATION(initVkDevice());

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
