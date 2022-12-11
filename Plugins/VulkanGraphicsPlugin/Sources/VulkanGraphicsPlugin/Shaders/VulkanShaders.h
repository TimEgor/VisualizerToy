#pragma once

#include "VulkanGraphicsPlugin/VulkanCore.h"

#include "ManagedGraphicDevice/ManagedObjects/ManagedShaderObjects.h"

namespace VT_VK
{
	class VulkanShaderBase
	{
		friend VulkanGraphicDevice;

	private:
		VkShaderModule m_vkShaderModule = 0;

	public:
		VulkanShaderBase(VkShaderModule vkShaderModule)
			: m_vkShaderModule(vkShaderModule) {}
		virtual ~VulkanShaderBase() = default;

		VkShaderModule getVkShaderModule() const { return m_vkShaderModule; }
	};

	class VulkanVertexShader final : public VT::ManagedGraphicDevice::ManagedVertexShaderBase, public VulkanShaderBase
	{
	public:
		VulkanVertexShader(VkShaderModule vkShaderModule)
			: VulkanShaderBase(vkShaderModule) {}
	};

	class VulkanPixelShader final : public VT::ManagedGraphicDevice::ManagedPixelShaderBase, public VulkanShaderBase
	{
	public:
		VulkanPixelShader(VkShaderModule vkShaderModule)
			: VulkanShaderBase(vkShaderModule) {}
	};
}
