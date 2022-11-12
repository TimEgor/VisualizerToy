#pragma once

#include "GraphicPlatform/IGraphicPlatform.h"
#include "VulkanGraphicsPlugin/VulkanCore.h"

#define VT_GRAPHIC_PLARFORM_VULKAN_TYPE

namespace VT_VK
{
	class VulkanGraphicPlatform final : public VT::IGraphicPlatform
	{
	private:
		VkInstance m_vkInstance = nullptr;
		bool m_isSwapChainEnabled = false;

		bool initVkInstance(bool isSwapChainEnabled);

	public:
		VulkanGraphicPlatform() = default;
		~VulkanGraphicPlatform() { release(); }

		virtual bool init(bool isSwapChainEnabled) override;
		virtual void release() override;

		VkInstance getInstance() const { return m_vkInstance; }

		virtual VT::IGraphicDevice* createGraphicDevice() override;

		VT_GRAPHIC_PLATFORM_TYPE(VT_GRAPHIC_PLARFORM_VULKAN_TYPE)
	};
}