#include "GraphicRenderContext.h"

#include "Textures/ITexture2D.h"
#include "MeshSystem/IMesh.h"

#include <cassert>

void VT::GraphicRenderContextUtils::setRenderingTargets(IRenderContext* context, uint32_t targetsCount,
	const GraphicRenderContextTarget* renderTargets, const DepthStencilContextTarget* depthStencilTarget)
{
	assert(context);

	IGraphicResourceDescriptor* rtvDescriptors[IRenderContext::MAX_RENDER_TARGETS_COUNT];
	Scissors scissors[IRenderContext::MAX_RENDER_TARGETS_COUNT];
	Viewport viewports[IRenderContext::MAX_RENDER_TARGETS_COUNT];

	for (uint32_t i = 0; i < targetsCount; ++i)
	{
		const GraphicRenderContextTarget& target = renderTargets[i];

		prepareTextureResourceForRendering(context, target.m_targetTexture);

		rtvDescriptors[i] = target.m_targetView;
		scissors[i] = target.m_scissors;
		viewports[i] = target.m_viewport;
	}

	const IGraphicResourceDescriptor* depthStencilView = nullptr;
	if (depthStencilTarget)
	{
		prepareTextureResourceForDepthStencilRendering(context, depthStencilTarget->m_targetTexture);
		depthStencilView = depthStencilTarget->m_targetView;
	}

	context->setRenderTargets(targetsCount, rtvDescriptors, depthStencilView);
	context->setScissors(targetsCount, scissors);
	context->setViewports(targetsCount, viewports);

	for (uint32_t i = 0; i < targetsCount; ++i)
	{
		const GraphicRenderContextTarget& target = renderTargets[i];
		context->clearRenderTarget(target.m_targetView, target.m_clearValue);
	}

	if (depthStencilTarget)
	{
		context->clearDepthStencilTarget(depthStencilView, depthStencilTarget->m_depthClearValue, depthStencilTarget->m_stencilClearValue);
	}

}

void VT::GraphicRenderContextUtils::prepareTextureForRendering(IRenderContext* context, Texture2DReference texture)
{
	prepareTextureResourceForRendering(context, texture->getTypedTexture());
}

void VT::GraphicRenderContextUtils::prepareTextureResourceForRendering(IRenderContext* context, ITexture2D* texture)
{
	assert(context);

	context->changeResourceState(texture, texture->getState(), TextureState::TEXTURE_STATE_RENDER_TARGET);
	texture->setState(TextureState::TEXTURE_STATE_RENDER_TARGET);
}

void VT::GraphicRenderContextUtils::prepareTextureResourceForDepthStencilRendering(IRenderContext* context,
	ITexture2D* texture)
{
	assert(context);

	context->changeResourceState(texture, texture->getState(), TextureState::TEXTURE_STATE_DEPTH_STENCIL);
	texture->setState(TextureState::TEXTURE_STATE_DEPTH_STENCIL);
}

void VT::GraphicRenderContextUtils::prepareTextureResourceForPresenting(IRenderContext* context, ITexture2D* texture)
{
	assert(context);

	context->changeResourceState(texture, texture->getState(), TextureState::TEXTURE_STATE_PRESENTING);
	texture->setState(TextureState::TEXTURE_STATE_PRESENTING);
}

void VT::GraphicRenderContextUtils::prepareResourceForBinding(IRenderContext* context,
	GraphicResourceReference resource, GraphicResourceStateValueType targetState)
{
}

void VT::GraphicRenderContextUtils::setPipelineState(IRenderContext* context, GraphicPipelineStateReference pipelineState)
{
	assert(context);
	context->setPipelineState(pipelineState->getTypedObject());
}

void VT::GraphicRenderContextUtils::setVertexBuffers(IRenderContext* context, uint32_t buffersCount,
	const GPUBufferReference* buffers, const InputLayoutDesc& inputLayoutDesc)
{
	assert(context);

	IGPUBuffer* bufferResources[IRenderContext::MAX_VERTEX_BUFFERS_COUNT];

	for (uint32_t i = 0; i < buffersCount; ++i)
	{
		bufferResources[i] = buffers[i]->getTypedResource();
		context->changeResourceState(bufferResources[i], bufferResources[i]->getState(), GPUBufferState::GPU_BUFFER_STATE_VERTEX_BUFFER);
	}

	context->setVertexBuffers(buffersCount, bufferResources, inputLayoutDesc);
}

void VT::GraphicRenderContextUtils::setIndexBuffer(IRenderContext* context, GPUBufferReference buffer,
	InputLayoutElementType indexType)
{
	assert(context);
	IGPUBuffer* indexBuffer = buffer->getTypedResource();
	context->changeResourceState(indexBuffer, indexBuffer->getState(), GPUBufferState::GPU_BUFFER_STATE_INDEX_BUFFER);
	context->setIndexBuffer(indexBuffer, indexType);
}
