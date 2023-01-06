#pragma once

#include "InputLayout/InputLayout.h"
#include "RenderSystem/IRenderContext.h"

namespace VT
{
	class IGPUBuffer;
}

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
		virtual void setVertexBuffer(VT::IGPUBuffer* buffer) override;
		virtual void setVertexBuffers(uint32_t buffersCount, VT::IGPUBuffer** buffers) override;
		virtual void setIndexBuffer(VT::IGPUBuffer* buffer, VT::InputLayoutElementType indexType) override;

		virtual void draw(uint32_t vertCount) override;
		virtual void drawIndexed(uint32_t indexCount) override;
	};
}
