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
}

void VT::GraphicRenderContextUtils::setRenderingTargets(IRenderContext* context, uint32_t targetsCount,
	const GraphicRenderContextTarget* renderTargets)
{
	setRenderingTargets(context, targetsCount, renderTargets, nullptr);
}

void VT::GraphicRenderContextUtils::setClearingRenderingTargets(IRenderContext* context, uint32_t targetsCount,
	const GraphicRenderContextTarget* renderTargets, const DepthStencilContextTarget* depthStencilTarget,
	const GraphicRenderTargetClearingValue* clearingValues, float depthClearingValue, uint32_t stencilClearinglValue)
{
	setRenderingTargets(context, targetsCount, renderTargets, depthStencilTarget);

	for (uint32_t i = 0; i < targetsCount; ++i)
	{
		const GraphicRenderContextTarget& target = renderTargets[i];
		context->clearRenderTarget(target.m_targetView, clearingValues[i].m_vals);
	}

	if (depthStencilTarget)
	{
		context->clearDepthStencilTarget(depthStencilTarget->m_targetView, depthClearingValue, stencilClearinglValue);
	}
}

void VT::GraphicRenderContextUtils::setClearingRenderingTargets(IRenderContext* context, uint32_t targetsCount,
	const GraphicRenderContextTarget* renderTargets, const GraphicRenderTargetClearingValue* clearingValues)
{
	setClearingRenderingTargets(context, targetsCount, renderTargets, nullptr, clearingValues, 0.0f, 0);
}

void VT::GraphicRenderContextUtils::clearRenderTargets(IRenderContext* context, uint32_t targetsCount,
	const IGraphicResourceDescriptor** targetViews, const GraphicRenderTargetClearingValue* clearingValues)
{
	for (uint32_t i = 0; i < targetsCount; ++i)
	{
		context->clearRenderTarget(targetViews[i], clearingValues[i].m_vals);
	}
}

void VT::GraphicRenderContextUtils::clearDepthStencil(IRenderContext* context,
	const IGraphicResourceDescriptor* depthStencilView, float depthClearingValue, uint32_t stencilClearinglValue)
{
	context->clearDepthStencilTarget(depthStencilView, depthClearingValue, stencilClearinglValue);
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
