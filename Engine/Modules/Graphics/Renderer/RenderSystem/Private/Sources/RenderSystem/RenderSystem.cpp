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
#include "InputLayout/IInputLayout.h"

#include "Math/Vector.h"

namespace VT
{
	// tmp vertex struct
	struct Vertex final
	{
		Vector2 m_pos;
		Vector3 m_color;
	};
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

	GPUBufferDesc vertexBufferDesc{};
	vertexBufferDesc.m_byteSize = sizeof(Vertex) * 3;
	vertexBufferDesc.m_usage = GPUBufferUsageType::VERTEX_BUFFER;

	m_drawingData.m_vertexBuffer = resManager->createGPUBuffer(vertexBufferDesc);

	VT_CHECK_INITIALIZATION(m_drawingData.m_vertexBuffer);



	Vertex* vertexMapping = nullptr;
	m_drawingData.m_vertexBuffer->getResource()->mapData(reinterpret_cast<void**>(&vertexMapping));

	vertexMapping[0].m_pos = { 0.0f, -0.5f };
	vertexMapping[0].m_color = { 1.0f, 0.0f, 0.0f };
	vertexMapping[1].m_pos = { 0.5f, 0.5f };
	vertexMapping[1].m_color = { 0.0f, 1.0f, 0.0f };
	vertexMapping[2].m_pos = { -0.5f, 0.5f };
	vertexMapping[2].m_color = { 0.0f, 0.0f, 1.0f };

	m_drawingData.m_vertexBuffer->getResource()->unmapData();

	InputLayoutDesc inputLayoutDesc{};
	InputLayoutBindingDesc& binding = inputLayoutDesc.m_bindings.emplace_back();
	binding.m_index = 0;
	binding.m_stride = sizeof(Vertex);
	binding.m_type = InputLayoutBindingType::VERTEX_BINDING;

	InputLayoutElementDesc& posAttrib = inputLayoutDesc.m_elements.emplace_back();
	posAttrib.m_index = 0;
	posAttrib.m_slot = 0;
	posAttrib.m_offset = 0;
	posAttrib.m_componentNum = 2;
	posAttrib.m_type = InputLayoutElementType::FLOAT32;

	InputLayoutElementDesc& colorAttrib = inputLayoutDesc.m_elements.emplace_back();
	colorAttrib.m_index = 1;
	colorAttrib.m_slot = 0;
	colorAttrib.m_offset = offsetof(Vertex, m_color);
	colorAttrib.m_componentNum = 3;
	colorAttrib.m_type = InputLayoutElementType::FLOAT32;

	m_drawingData.m_inputLayout = resManager->addInputLayout(inputLayoutDesc);

	VT_CHECK_INITIALIZATION(m_drawingData.m_inputLayout);

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

	pipelineStateInfo.m_formats.push_back(target->getDesc().m_format);

	PipelineStateReference pipelineState = environment->m_graphicResourceManager->getPipelineState(pipelineStateInfo, m_drawingData.m_inputLayout);

	RenderContextBeginInfo contextInfo{};
	contextInfo.m_targets.push_back({ target, targetView });

	m_context->begin();
	m_context->prepareTextureForRendering(target);
	m_context->beginRendering(contextInfo);

	m_context->setPipelineState(pipelineState->getResource());
	m_context->setVertexBuffer(m_drawingData.m_vertexBuffer->getResource());
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
