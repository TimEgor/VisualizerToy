#pragma once

#include "InputLayout/InputLayout.h"
#include "RenderSystem/IRenderContext.h"

namespace VT_D3D12
{
	class D3D12GraphicsCommandList;

	class D3D12RenderContext final : public VT::IRenderContext
	{
	private:
		D3D12GraphicsCommandList* m_commandList = nullptr;

	public:
		D3D12RenderContext() = default;
		~D3D12RenderContext() { release(); }

		virtual bool init(VT::ICommandList* commandList) override;
		virtual void release() override;

		virtual VT::ICommandList* getCommandList() override;

		virtual void begin() override;
		virtual void end() override;

		virtual void beginRendering(const VT::RenderContextBeginInfo& info) override;
		virtual void endRendering() override;

		virtual void prepareTextureForRendering(VT::ITexture2D* texture) override;
		virtual void prepareTextureForPresenting(VT::ITexture2D* texture) override;

		virtual void setDescriptorHeap(VT::IGraphicResourceDescriptorHeap* heap) override;

		virtual void setBindingParameterValue(uint32_t index, uint32_t offset, uint32_t value) override;
		virtual void setBindingParameterValues(uint32_t index, uint32_t offset, uint32_t valuesCount, uint32_t* values) override;
		virtual void setBindingLayout(const VT::IPipelineBindingLayout* bindingLayout) override;

		virtual void setPipelineState(const VT::IPipelineState* pipelineState) override;
		virtual void setVertexBuffers(uint32_t buffersCount, VT::IGPUBuffer** buffers, const VT::InputLayoutDesc& inputLayoutDesc) override;
		virtual void setIndexBuffer(VT::IGPUBuffer* buffer, VT::InputLayoutElementType indexType) override;

		virtual void draw(uint32_t vertCount) override;
		virtual void drawIndexed(uint32_t indexCount) override;
	};
}
