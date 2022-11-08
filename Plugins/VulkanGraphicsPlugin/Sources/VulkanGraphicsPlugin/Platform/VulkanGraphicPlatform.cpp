#include "VulkanGraphicPlatform.h"

#include "Core/UtilitiesMacros.h"

#include "VulkanGraphicsPlugin/Device/VulkanGraphicDevice.h"
#include "VulkanGraphicsPlugin/ResourceContainer/VulkanResourceContainer.h"

namespace VT_VK
{
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
}

bool VT_VK::VulkanGraphicPlatform::initVkInstance(bool isSwapChainEnabled)
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
	if (isSwapChainEnabled)
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

bool VT_VK::VulkanGraphicPlatform::init(bool isSwapChainEnabled)
{
	VT_CHECK_INITIALIZATION(initVkInstance(isSwapChainEnabled));

	volkLoadInstance(m_vkInstance);

	return true;
}

void VT_VK::VulkanGraphicPlatform::release()
{
	if (m_vkInstance)
	{
		vkDestroyInstance(m_vkInstance, nullptr);
	}
}

VT::IGraphicDevice* VT_VK::VulkanGraphicPlatform::createGraphicDevice()
{
	return new VulkanGraphicDevice();
}

VT::SwapChainContainer* VT_VK::VulkanGraphicPlatform::createSwapChainResourceContainer()
{
	return new VulkanSwapChainContainer();
}

VT::Texture2DContainer* VT_VK::VulkanGraphicPlatform::createTexture2DResourceContainer()
{
	return new VulkanTexture2DContainer();
}
