#pragma once

#include "VulkanGraphicsPlugin/VulkanCore.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedPipelineObjects.h"

namespace VT_VK
{
	class VulkanPipelineState final : public VT::ManagedGraphicDevice::ManagedPipelineStateBase
	{
		friend class VulkanGraphicDevice;

	private:
		VkPipeline m_vkPipeline = 0;

		VT::PipelineStateHash m_hash = 0;

	public:
		VulkanPipelineState(VkPipeline vkPipeline, VT::PipelineStateHash hash)
			: m_vkPipeline(vkPipeline), m_hash(hash) {}

		VkPipeline getVkPipeline() const { return m_vkPipeline; }

		virtual VT::PipelineStateHash getHash() const override { return m_hash; }
	};
}