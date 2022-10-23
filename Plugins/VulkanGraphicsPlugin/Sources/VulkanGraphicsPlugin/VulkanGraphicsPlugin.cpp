#include "VulkanGraphicsPlugin.h"

#include "Device/VulkanGraphicDevice.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

VT_PLUGIN(VulkanGraphicPlugin)

void VulkanGraphicPlugin::onLoaded()
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

	environment->m_graphicDevice = new VulkanGraphicDevice();
}

void VulkanGraphicPlugin::onUnloaded()
{}
