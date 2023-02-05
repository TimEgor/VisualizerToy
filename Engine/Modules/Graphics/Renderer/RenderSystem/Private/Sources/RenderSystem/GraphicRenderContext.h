#pragma once

#include "RenderContext/IRenderContext.h"
#include "GraphicResourceManager/ObjectHandles.h"

namespace VT
{
	class ICommandList;
	class IGraphicResourceDescriptorHeap;

	struct RenderContextTarget final
	{
		ITexture2D* m_targetTexture = nullptr;
		IGraphicResourceDescriptor* m_targetView = nullptr;

		Viewport m_viewport;
		Scissors m_scissors;

		float clearValue[4];
	};

	class GraphicRenderContext final
	{
	private:
		IRenderContext* m_context = nullptr;

	public:
		GraphicRenderContext() = default;

		bool init(IRenderContext* context);
		void release();

		IRenderContext* getRenderContext() { return m_context; }

		void setRenderingTargets(uint32_t targetsCount, const RenderContextTarget* targets);

		void prepareTextureForRendering(Texture2DReference texture);
		void prepareTextureResourceForPresenting(ITexture2D* texture);
		void prepareTextureResourceForRendering(ITexture2D* texture);
		void prepareResourceForBinding(GraphicResourceReference resource, GraphicStateValueType targetState);

		void setPipelineState(PipelineStateReference pipelineState);
		void setVertexBuffers(uint32_t buffersCount, const GPUBufferReference* buffers, const InputLayoutDesc& inputLayoutDesc);
		void setIndexBuffer(GPUBufferReference buffer, InputLayoutElementType indexType);
	};
}
