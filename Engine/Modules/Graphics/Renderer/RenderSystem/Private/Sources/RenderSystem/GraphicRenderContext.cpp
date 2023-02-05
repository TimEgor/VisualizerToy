#include "GraphicRenderContext.h"

#include "Textures/ITexture2D.h"
#include "MeshSystem/IMesh.h"

#include <cassert>


bool VT::GraphicRenderContext::init(IRenderContext* context)
{
	m_context = context;
	return context;
}

void VT::GraphicRenderContext::release()
{
	m_context = nullptr;
}

void VT::GraphicRenderContext::setRenderingTargets(uint32_t targetsCount, const RenderContextTarget* targets)
{
	assert(m_context);

	IGraphicResourceDescriptor* rtvDescriptors[IRenderContext::MAX_RENDER_TARGETS_COUNT];
	Scissors scissors[IRenderContext::MAX_RENDER_TARGETS_COUNT];
	Viewport viewports[IRenderContext::MAX_RENDER_TARGETS_COUNT];

	for (uint32_t i = 0; i < targetsCount; ++i)
	{
		const RenderContextTarget& target = targets[i];

		prepareTextureResourceForRendering(target.m_targetTexture);

		rtvDescriptors[i] = target.m_targetView;
		scissors[i] = target.m_scissors;
		viewports[i] = target.m_viewport;
	}

	m_context->setRenderTargets(targetsCount, rtvDescriptors);
	m_context->setScissors(targetsCount, scissors);
	m_context->setViewports(targetsCount, viewports);

	for (uint32_t i = 0; i < targetsCount; ++i)
	{
		const RenderContextTarget& target = targets[i];
		m_context->clearRenderTarget(target.m_targetView, target.clearValue);
	}
}

void VT::GraphicRenderContext::prepareTextureForRendering(Texture2DReference texture)
{
	prepareTextureResourceForRendering(texture->getTypeTexture());
}

void VT::GraphicRenderContext::prepareTextureResourceForRendering(ITexture2D* texture)
{
	assert(m_context);

	m_context->changeResourceState(texture, texture->getState(), TextureState::RENDER_TARGET);
	texture->setState(TextureState::RENDER_TARGET);
}

void VT::GraphicRenderContext::prepareTextureResourceForPresenting(ITexture2D* texture)
{
	assert(m_context);

	m_context->changeResourceState(texture, texture->getState(), TextureState::PRESENTING);
	texture->setState(TextureState::PRESENTING);
}


void VT::GraphicRenderContext::prepareResourceForBinding(GraphicResourceReference resource,
	GraphicStateValueType targetState)
{
}

void VT::GraphicRenderContext::setPipelineState(PipelineStateReference pipelineState)
{
	assert(m_context);
	m_context->setPipelineState(pipelineState->getTypedObject());
}

void VT::GraphicRenderContext::setVertexBuffers(uint32_t buffersCount, const GPUBufferReference* buffers,
	const InputLayoutDesc& inputLayoutDesc)
{
	assert(m_context);

	IGPUBuffer* bufferResources[IRenderContext::MAX_VERTEX_BUFFERS_COUNT];

	for (uint32_t i = 0; i < buffersCount; ++i)
	{
		bufferResources[i] = buffers[i]->getTypedResource();
		m_context->changeResourceState(bufferResources[i], bufferResources[i]->getState(), GPUBufferState::VERTEX_BUFFER);
	}

	m_context->setVertexBuffers(buffersCount, bufferResources, inputLayoutDesc);
}

void VT::GraphicRenderContext::setIndexBuffer(GPUBufferReference buffer, InputLayoutElementType indexType)
{
	assert(m_context);
	IGPUBuffer* indexBuffer = buffer->getTypedResource();
	m_context->changeResourceState(indexBuffer, indexBuffer->getState(), GPUBufferState::INDEX_BUFFER);
	m_context->setIndexBuffer(indexBuffer, indexType);
}
