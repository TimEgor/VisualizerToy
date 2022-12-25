#pragma once

#include "RenderSystem/IRenderContext.h"

namespace VT_VK
{
	class VulkanCommandList;

	class VulkanRenderContext final : public VT::IRenderContext
	{
	private:
		VulkanCommandList* m_commandList = nullptr;

	public:
		VulkanRenderContext() = default;
		~VulkanRenderContext() { release(); }

		virtual bool init(VT::ICommandList* commandList) override;
		virtual void release() override;

		virtual VT::ICommandList* getCommandList() override;

		virtual void begin() override;
		virtual void end() override;

		virtual void beginRendering(const VT::RenderContextBeginInfo& info) override;
		virtual void endRendering() override;

		virtual void prepareTextureForRendering(VT::ITexture2D* texture) override;
		virtual void prepareTextureForPresenting(VT::ITexture2D* texture) override;

		virtual void setPipelineState(VT::IPipelineState* pipelineState) override;

		virtual void draw() override;
	};
}
