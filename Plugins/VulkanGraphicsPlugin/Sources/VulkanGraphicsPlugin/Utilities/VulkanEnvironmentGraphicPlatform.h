#pragma once

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "VulkanGraphicsPlugin/Platform/VulkanGraphicPlatform.h"

namespace VT_VK
{
	inline VulkanGraphicPlatform* getVulkanEnvironmentGraphicPlatform()
	{
		VT::IGraphicPlatform* platform = VT::EngineInstance::getInstance()->getEnvironment()->m_graphicPlatform;
		assert(platform->getType() == VulkanGraphicPlatform::getGraphicPlatformType());

		return reinterpret_cast<VulkanGraphicPlatform*>(platform);
	}
}