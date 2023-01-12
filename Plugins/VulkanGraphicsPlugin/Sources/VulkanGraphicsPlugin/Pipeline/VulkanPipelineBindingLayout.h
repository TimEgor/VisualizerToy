#pragma once

#include "VulkanGraphicsPlugin/VulkanCore.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedPipelineObjects.h"

namespace VT_VK
{
	class VulkanPipelineBindingLayout final : public VT::ManagedGraphicDevice::ManagedPipelineBindingLayoutBase
	{
		friend class VulkanGraphicDevice;

	private:
		VkPipelineLayout m_vkPipelineLayout = 0;

		VT::PipelineBindingLayoutHash m_hash = 0;

	public:
		VulkanPipelineBindingLayout(VkPipelineLayout vkPipelineLayout, VT::PipelineBindingLayoutHash hash)
			: m_vkPipelineLayout(vkPipelineLayout), m_hash(hash) {}

		VkPipelineLayout getVkPipelineLayout() const { return m_vkPipelineLayout; }

		virtual VT::PipelineBindingLayoutHash getHash() const override { return m_hash; }
	};
}