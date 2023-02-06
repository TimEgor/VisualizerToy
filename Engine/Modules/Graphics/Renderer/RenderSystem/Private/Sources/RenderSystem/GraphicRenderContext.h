#pragma once

#include "RenderContext/IRenderContext.h"
#include "GraphicResourceManager/ObjectHandles.h"

namespace VT
{
	struct RenderContextTarget final
	{
		ITexture2D* m_targetTexture = nullptr;
		IGraphicResourceDescriptor* m_targetView = nullptr;

		Viewport m_viewport;
		Scissors m_scissors;

		float clearValue[4];
	};

	namespace GraphicRenderContextUtils
	{
		void setRenderingTargets(IRenderContext* context, uint32_t targetsCount, const RenderContextTarget* targets);

		void prepareTextureForRendering(IRenderContext* context, Texture2DReference texture);
		void prepareTextureResourceForPresenting(IRenderContext* context, ITexture2D* texture);
		void prepareTextureResourceForRendering(IRenderContext* context, ITexture2D* texture);
		void prepareResourceForBinding(IRenderContext* context, GraphicResourceReference resource, GraphicStateValueType targetState);

		void setPipelineState(IRenderContext* context, PipelineStateReference pipelineState);
		void setVertexBuffers(IRenderContext* context, uint32_t buffersCount, const GPUBufferReference* buffers, const InputLayoutDesc& inputLayoutDesc);
		void setIndexBuffer(IRenderContext* context, GPUBufferReference buffer, InputLayoutElementType indexType);
	}
}
