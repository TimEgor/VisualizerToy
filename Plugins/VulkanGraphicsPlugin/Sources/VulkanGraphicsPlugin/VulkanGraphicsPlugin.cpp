#include "VulkanGraphicsPlugin.h"

#include "Device/VulkanGraphicDevice.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

VT_PLUGIN(VT_VK::VulkanGraphicPlugin)

void VT_VK::VulkanGraphicPlugin::onLoaded()
{
	VT::EngineEnvironment* environment = VT::EngineInstance::getInstance()->getEnvironment();
	if (!environment)
	{
		assert(false && "VulkanGraphicPlugin::onLoaded() : Engine environment is invalid.");
		return;
	}

	if (environment->m_graphicDevice)
	{
		assert(false && "VulkanGraphicPlugin::onLoaded() : Graphic device has been already initialized.");
		return;
	}

	volkInitialize();

	environment->m_graphicDevice = new VulkanGraphicDevice();
}

void VT_VK::VulkanGraphicPlugin::onUnloaded()
{
	VT::EngineEnvironment* environment = VT::EngineInstance::getInstance()->getEnvironment();
	if (!environment)
	{
		assert(false && "VulkanGraphicPlugin::onUnloaded() : Engine environment is invalid.");
		return;
	}

	VT::IGraphicDevice* graphicDevice = environment->m_graphicDevice;

	if (graphicDevice && graphicDevice->getType() == VulkanGraphicDevice::getGraphicDeviceType())
	{
		VT_SAFE_DESTROY_WITH_RELEASING(graphicDevice);
	}
}
