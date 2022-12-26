#include "RenderSystem.h"

#include "../../../../../Device/Textures/Sources/Textures/ITexture2D.h"
#include "GraphicDevice/IGraphicDevice.h"
#include "GraphicPlatform/IGraphicPlatform.h"
#include "GraphicPipeline/IPipelineState.h"
#include "RenderSystem/IRenderContext.h"
#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"
#include "GraphicResourceManager/IGraphicResourceManager.h"

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
	m_drawingData.vertShader = resManager->loadVertexShader("TestVertexShader.hlsl");
	m_drawingData.pixelShader = resManager->loadPixelShader("TestPixelShader.hlsl");

	VT_CHECK_INITIALIZATION(m_drawingData.vertShader && m_drawingData.pixelShader);

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
	pipelineStateInfo.m_vertexShader = m_drawingData.vertShader->getResource();
	pipelineStateInfo.m_pixelShader = m_drawingData.pixelShader->getResource();

	pipelineStateInfo.m_formats.push_back(target->getDesc().m_format);

	PipelineStateReference pipelineState = environment->m_graphicResourceManager->getPipelineState(pipelineStateInfo);

	RenderContextBeginInfo contextInfo{};
	contextInfo.m_targets.push_back({ target, targetView });

	m_context->begin();
	m_context->prepareTextureForRendering(target);
	m_context->beginRendering(contextInfo);

	m_context->setPipelineState(pipelineState->getResource());
	m_context->draw();

	m_context->endRendering();
	m_context->prepareTextureForPresenting(target);
	m_context->end();

	CommandListSubmitInfo submitInfo{};
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
