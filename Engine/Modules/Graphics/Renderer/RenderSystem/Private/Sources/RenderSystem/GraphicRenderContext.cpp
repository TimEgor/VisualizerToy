#include "GraphicRenderContext.h"

#include "Textures/ITexture2D.h"
#include "MeshSystem/IMesh.h"

#include <cassert>

void VT::GraphicRenderContextUtils::setRenderingTargets(IRenderContext* context, uint32_t targetsCount,
	const RenderContextTarget* targets)
{
	assert(context);

	IGraphicResourceDescriptor* rtvDescriptors[IRenderContext::MAX_RENDER_TARGETS_COUNT];
	Scissors scissors[IRenderContext::MAX_RENDER_TARGETS_COUNT];
	Viewport viewports[IRenderContext::MAX_RENDER_TARGETS_COUNT];

	for (uint32_t i = 0; i < targetsCount; ++i)
	{
		const RenderContextTarget& target = targets[i];

		prepareTextureResourceForRendering(context, target.m_targetTexture);

		rtvDescriptors[i] = target.m_targetView;
		scissors[i] = target.m_scissors;
		viewports[i] = target.m_viewport;
	}

	context->setRenderTargets(targetsCount, rtvDescriptors);
	context->setScissors(targetsCount, scissors);
	context->setViewports(targetsCount, viewports);

	for (uint32_t i = 0; i < targetsCount; ++i)
	{
		const RenderContextTarget& target = targets[i];
		context->clearRenderTarget(target.m_targetView, target.clearValue);
	}
}

void VT::GraphicRenderContextUtils::prepareTextureForRendering(IRenderContext* context, Texture2DReference texture)
{
	prepareTextureResourceForRendering(context, texture->getTypeTexture());
}

void VT::GraphicRenderContextUtils::prepareTextureResourceForRendering(IRenderContext* context, ITexture2D* texture)
{
	assert(context);

	context->changeResourceState(texture, texture->getState(), TextureState::RENDER_TARGET);
	texture->setState(TextureState::RENDER_TARGET);
}

void VT::GraphicRenderContextUtils::prepareTextureResourceForPresenting(IRenderContext* context, ITexture2D* texture)
{
	assert(context);

	context->changeResourceState(texture, texture->getState(), TextureState::PRESENTING);
	texture->setState(TextureState::PRESENTING);
}

void VT::GraphicRenderContextUtils::prepareResourceForBinding(IRenderContext* context,
	GraphicResourceReference resource, GraphicStateValueType targetState)
{
}

void VT::GraphicRenderContextUtils::setPipelineState(IRenderContext* context, PipelineStateReference pipelineState)
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
		context->changeResourceState(bufferResources[i], bufferResources[i]->getState(), GPUBufferState::VERTEX_BUFFER);
	}

	context->setVertexBuffers(buffersCount, bufferResources, inputLayoutDesc);
}

void VT::GraphicRenderContextUtils::setIndexBuffer(IRenderContext* context, GPUBufferReference buffer,
	InputLayoutElementType indexType)
{
	assert(context);
	IGPUBuffer* indexBuffer = buffer->getTypedResource();
	context->changeResourceState(indexBuffer, indexBuffer->getState(), GPUBufferState::INDEX_BUFFER);
	context->setIndexBuffer(indexBuffer, indexType);
}
