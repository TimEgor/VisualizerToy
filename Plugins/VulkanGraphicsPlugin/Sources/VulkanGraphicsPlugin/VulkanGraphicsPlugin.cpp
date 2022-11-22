#include "VulkanGraphicsPlugin.h"

#include "Platform/VulkanGraphicPlatform.h"
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

	if (environment->m_graphicPlatform)
	{
		assert(false && "VulkanGraphicPlugin::onLoaded() : Graphic platform has been already initialized.");
		return;
	}

	volkInitialize();

	environment->m_graphicPlatform = new VulkanGraphicPlatform();
}

void VT_VK::VulkanGraphicPlugin::onUnloaded()
{
	VT::EngineEnvironment* environment = VT::EngineInstance::getInstance()->getEnvironment();
	if (!environment)
	{
		assert(false && "VulkanGraphicPlugin::onUnloaded() : Engine environment is invalid.");
		return;
	}

	VT::IGraphicPlatform* graphicPlatform = environment->m_graphicPlatform;

	if (!graphicPlatform)
	{
		assert(false && "VulkanGraphicPlugin::onUnloaded() : Engine environment graphicPlatform is null.");
		return;
	}
	
	if(graphicPlatform->getType() != VulkanGraphicPlatform::getGraphicPlatformType())
	{
		assert(false && "VulkanGraphicPlugin::onUnloaded() : Engine environment graphicPlatform isn't Vulkan.");
		return;
	}

	VT_SAFE_DESTROY_WITH_RELEASING(graphicPlatform);
}
