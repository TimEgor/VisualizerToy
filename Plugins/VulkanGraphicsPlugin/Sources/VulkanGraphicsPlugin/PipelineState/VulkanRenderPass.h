#pragma once

#include "VulkanGraphicsPlugin/VulkanCore.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedRenderPassObject.h"

namespace VT_VK
{
	class VulkanRenderPass final : public VT::ManagedGraphicDevice::ManagedRenderPassBase
	{
		friend class VulkanGraphicDevice;

	private:
		VkRenderPass m_vkRenderPass = 0;

		VT::RenderPassHash m_hash = 0;

	public:
		VulkanRenderPass(VkRenderPass vkRenderPass, VT::RenderPassHash hash)
			: m_vkRenderPass(vkRenderPass), m_hash(hash) {}

		VkRenderPass getVkRenderPass() const { return m_vkRenderPass; }

		virtual VT::RenderPassHash getHash() const override { return m_hash; }
	};
}