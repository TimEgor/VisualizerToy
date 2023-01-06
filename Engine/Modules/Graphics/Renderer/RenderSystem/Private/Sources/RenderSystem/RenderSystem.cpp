#include "RenderSystem.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "GraphicDevice/IGraphicDevice.h"
#include "GraphicPlatform/IGraphicPlatform.h"
#include "GraphicResourceManager/IGraphicResourceManager.h"
#include "RenderSystem/IRenderContext.h"

#include "GraphicPipeline/IPipelineState.h"
#include "Textures/ITexture2D.h"
#include "GPUBuffer/IGPUBuffer.h"

#include "Math/Vector.h"
#include "MeshSystem/IMeshSystem.h"

namespace VT
{
	// tmp vertex struct
	struct Vertex final
	{
		Vector2 m_pos;
		Vector3 m_color;
	};

	using VertexIndex = uint16_t;
}


bool VT::RenderSystem::init()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	m_commandPool = environment->m_graphicDevice->createCommandPool();
	VT_CHECK_INITIALIZATION(m_commandPool);

	ICommandList* commandList = environment->m_graphicDevice->allocateCommandList(m_commandPool);
	VT_CHECK_INITIALIZATION(commandList);

	m_context = environment->m_graphicPlatform->createRenderContext();
	VT_CHECK_INITIALIZATION(m_context && m_context->init(commandList));

	m_frameFence = environment->m_graphicDevice->createFence(true);
	VT_CHECK_INITIALIZATION(m_frameFence);

	m_renderingCompleteSemaphore = environment->m_graphicDevice->createSemaphore();
	VT_CHECK_INITIALIZATION(m_renderingCompleteSemaphore);

	IGraphicResourceManager* resManager = environment->m_graphicResourceManager;
	m_drawingData.m_vertShader = resManager->loadVertexShader("TestInputLayoutVertexShader.hlsl");
	m_drawingData.m_pixelShader = resManager->loadPixelShader("TestInputLayoutPixelShader.hlsl");

	VT_CHECK_INITIALIZATION(m_drawingData.m_vertShader && m_drawingData.m_pixelShader);

	m_drawingData.m_mesh = environment->m_meshSystem->loadMesh("Cube.mesh");

	VT_CHECK_INITIALIZATION(m_drawingData.m_mesh);

	return true;
}

void VT::RenderSystem::release()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	if (m_commandPool)
	{
		environment->m_graphicDevice->destroyCommandPool(m_commandPool);
		m_commandPool = nullptr;
	}

	if (m_frameFence)
	{
		environment->m_graphicDevice->destroyFence(m_frameFence);
		m_frameFence = nullptr;
	}

	if (m_renderingCompleteSemaphore)
	{
		environment->m_graphicDevice->destroySemaphore(m_renderingCompleteSemaphore);
		m_renderingCompleteSemaphore = nullptr;
	}

	VT_SAFE_DESTROY_WITH_RELEASING(m_context);
}

void VT::RenderSystem::render(ITexture2D* target, ITexture2DView* targetView,
	ISemaphore* textureAvailableSemaphore)
{
	assert(m_context);

	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	PipelineStateInfo pipelineStateInfo{};
	pipelineStateInfo.m_vertexShader = m_drawingData.m_vertShader->getResource();
	pipelineStateInfo.m_pixelShader = m_drawingData.m_pixelShader->getResource();

	IMesh* mesh = m_drawingData.m_mesh->getMesh();
	if (!mesh)
	{
		return;
	}

	const MeshVertexData& vertexData = mesh->getVertexData();
	const MeshIndexData& indexData = mesh->getIndexData();

	pipelineStateInfo.m_formats.push_back(target->getDesc().m_format);

	PipelineStateReference pipelineState = environment->m_graphicResourceManager->getPipelineState(pipelineStateInfo, vertexData.m_inputLayout);

	const uint32_t vertBuffersCount = static_cast<uint32_t>(vertexData.m_bindings.size());
	std::vector<IGPUBuffer*> vertBuffers;
	vertBuffers.reserve(vertBuffersCount);

	for (uint32_t i = 0; i < vertBuffersCount; ++i)
	{
		vertBuffers.push_back(vertexData.m_bindings[i]->getResource());
	}

	RenderContextBeginInfo contextInfo{};
	contextInfo.m_targets.push_back({ target, targetView });

	m_context->begin();
	m_context->prepareTextureForRendering(target);
	m_context->beginRendering(contextInfo);

	m_context->setPipelineState(pipelineState->getResource());
	m_context->setVertexBuffers(vertBuffers.size(), vertBuffers.data());
	m_context->setIndexBuffer(indexData.m_indexBuffer->getResource(), indexData.m_indexFormat);
	m_context->drawIndexed(indexData.m_indexCount);

	m_context->endRendering();
	m_context->prepareTextureForPresenting(target);
	m_context->end();

	CommandListSubmitInfo submitInfo;
	submitInfo.m_fence = m_frameFence;
	submitInfo.m_waitSemaphore = textureAvailableSemaphore;
	submitInfo.m_completeSemaphore = m_renderingCompleteSemaphore;

	environment->m_graphicDevice->submitCommandList(m_context->getCommandList(), submitInfo);
}

void VT::RenderSystem::waitFrame()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	environment->m_graphicDevice->waitFences(1, m_frameFence);
	environment->m_graphicDevice->resetFences(1, m_frameFence);
}

VT::ISemaphore* VT::RenderSystem::getRenderCompletedSemaphore()
{
	return m_renderingCompleteSemaphore;
}
