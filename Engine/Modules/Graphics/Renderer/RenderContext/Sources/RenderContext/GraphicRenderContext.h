#pragma once

#include "RenderContext/IRenderContext.h"
#include "GraphicResourceManager/ObjectHandles.h"

namespace VT
{
	struct GraphicRenderContextTarget final
	{
		ITexture2D* m_targetTexture = nullptr;
		IGraphicResourceDescriptor* m_targetView = nullptr;

		Viewport m_viewport;
		Scissors m_scissors;
	};

	struct DepthStencilContextTarget final
	{
		ITexture2D* m_targetTexture = nullptr;
		IGraphicResourceDescriptor* m_targetView = nullptr;
	};

	struct GraphicRenderTargetClearingValue final
	{
		float m_vals[4];
	};

	namespace GraphicRenderContextUtils
	{
		void setRenderingTargets(IRenderContext* context, uint32_t targetsCount,
			const GraphicRenderContextTarget* renderTargets, const DepthStencilContextTarget* depthStencilTarget);
		void setRenderingTargets(IRenderContext* context, uint32_t targetsCount,
			const GraphicRenderContextTarget* renderTargets);
		void setClearingRenderingTargets(IRenderContext* context, uint32_t targetsCount,
			const GraphicRenderContextTarget* renderTargets, const DepthStencilContextTarget* depthStencilTarget,
			const GraphicRenderTargetClearingValue* clearingValues, float depthClearingValue, uint32_t stencilClearinglValue);
		void setClearingRenderingTargets(IRenderContext* context, uint32_t targetsCount,
			const GraphicRenderContextTarget* renderTargets, const GraphicRenderTargetClearingValue* clearingValues);

		void clearRenderTargets(IRenderContext* context, uint32_t targetsCount, const IGraphicResourceDescriptor** targetViews,
			const GraphicRenderTargetClearingValue* clearingValues);
		void clearDepthStencil(IRenderContext* context, const IGraphicResourceDescriptor* depthStencilView,
			float depthClearingValue, uint32_t stencilClearinglValue);

		void prepareTextureForRendering(IRenderContext* context, Texture2DReference texture);
		void prepareTextureResourceForPresenting(IRenderContext* context, ITexture2D* texture);
		void prepareTextureResourceForRendering(IRenderContext* context, ITexture2D* texture);
		void prepareTextureResourceForDepthStencilRendering(IRenderContext* context, ITexture2D* texture);
		void prepareResourceForBinding(IRenderContext* context, GraphicResourceReference resource, GraphicResourceStateValueType targetState);

		void setPipelineState(IRenderContext* context, GraphicPipelineStateReference pipelineState);
		void setVertexBuffers(IRenderContext* context, uint32_t buffersCount, const GPUBufferReference* buffers, const InputLayoutDesc& inputLayoutDesc);
		void setIndexBuffer(IRenderContext* context, GPUBufferReference buffer, InputLayoutElementType indexType);
	}
}
