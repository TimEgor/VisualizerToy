#pragma once

#include "VulkanGraphicsPlugin/VulkanCore.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedPipelineStateObject.h"

namespace VT_VK
{
	class VulkanPipelineState final : public VT::ManagedGraphicDevice::ManagedPipelineStateBase
	{
		friend class VulkanGraphicDevice;

	private:
		VkPipeline m_vkPipeline = 0;
		VkPipelineLayout m_vkPipelineLayout = 0;

		VT::PipelineStateHash m_hash = 0;

	public:
		VulkanPipelineState(VkPipeline vkPipeline, VkPipelineLayout vkPipelineLayout, VT::PipelineStateHash hash)
			: m_vkPipeline(vkPipeline), m_vkPipelineLayout(vkPipelineLayout), m_hash(hash) {}

		VkPipeline getVkPipeline() const { return m_vkPipeline; }
		VkPipelineLayout getVkPipelineLayout() const { return m_vkPipelineLayout; }

		virtual VT::PipelineStateHash getHash() const override { return m_hash; }
	};
}