#include "RenderSystem.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "GraphicDevice/IGraphicDevice.h"
#include "GraphicPlatform/IGraphicPlatform.h"
#include "GraphicResourceManager/IGraphicResourceManager.h"
#include "RenderSystem/IRenderContext.h"

#include "GraphicPipeline/IPipelineState.h"
#include "GraphicPipeline/IPipelineBindingLayout.h"
#include "Textures/ITexture2D.h"
#include "GPUBuffer/IGPUBuffer.h"

#include "Math/Vector.h"
#include "MeshSystem/IMeshSystem.h"


bool VT::RenderSystem::init()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	ICommandList* commandList = environment->m_graphicDevice->createCommandList();
	VT_CHECK_INITIALIZATION(commandList);

	m_context = environment->m_graphicPlatform->createRenderContext();
	VT_CHECK_INITIALIZATION(m_context && m_context->init(commandList));

	m_frameFence = environment->m_graphicDevice->createFence();
	VT_CHECK_INITIALIZATION(m_frameFence);

	////

	IGraphicResourceManager* resManager = environment->m_graphicResourceManager;
	m_drawingData.m_vertShader = resManager->loadVertexShader("TestInputLayoutVertexShader.hlsl");
	m_drawingData.m_pixelShader = resManager->loadPixelShader("TestInputLayoutPixelShader.hlsl");

	VT_CHECK_INITIALIZATION(m_drawingData.m_vertShader && m_drawingData.m_pixelShader);

	m_drawingData.m_mesh = environment->m_meshSystem->loadMesh("Cube.mesh");

	VT_CHECK_INITIALIZATION(m_drawingData.m_mesh);

	////

	PipelineBindingLayoutDesc bindingDesc{};
	//bindingDesc.m_constDescriptorBindings.emplace_back(1, 0, 0, ShaderStageVisibility::ALL_STAGES);
	//bindingDesc.m_constDescriptorBindings.emplace_back(1, 1, 0, ShaderStageVisibility::VERTEX_STAGE);

	m_drawingPassData.m_bindingLayout = resManager->getPipelineBindingLayout(bindingDesc);

	return true;
}

void VT::RenderSystem::release()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	if (m_frameFence)
	{
		environment->m_graphicDevice->destroyFence(m_frameFence);
		m_frameFence = nullptr;
	}

	VT_SAFE_DESTROY_WITH_RELEASING(m_context);
}

void VT::RenderSystem::render(ITexture2D* target, IGraphicResourceDescriptor* targetView)
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

	const Texture2DDesc& targetDesc = target->getDesc();

	pipelineStateInfo.m_formats.push_back(targetDesc.m_format);

	PipelineStateReference pipelineState = environment->m_graphicResourceManager->getPipelineState(
		pipelineStateInfo, m_drawingPassData.m_bindingLayout, vertexData.m_inputLayout);

	const uint32_t vertBuffersCount = static_cast<uint32_t>(vertexData.m_bindings.size());
	std::vector<IGPUBuffer*> vertBuffers;
	vertBuffers.reserve(vertBuffersCount);

	for (uint32_t i = 0; i < vertBuffersCount; ++i)
	{
		vertBuffers.push_back(vertexData.m_bindings[i]->getResource());
	}

	RenderContextBeginInfo contextInfo{};
	contextInfo.m_targets.push_back({ target, targetView,
		Viewport(targetDesc.m_width, targetDesc.m_height),
		Scissors(targetDesc.m_width, targetDesc.m_height)
	});

	m_context->begin();
	m_context->prepareTextureForRendering(target);
	m_context->beginRendering(contextInfo);

	m_context->setPipelineState(pipelineState->getResource(), m_drawingPassData.m_bindingLayout->getResource());
	m_context->setVertexBuffers(vertBuffers.size(), vertBuffers.data(), vertexData.m_inputLayout->getDesc());
	m_context->setIndexBuffer(indexData.m_indexBuffer->getResource(), indexData.m_indexFormat);
	m_context->drawIndexed(indexData.m_indexCount);
	//m_context->draw(3);

	m_context->endRendering();
	m_context->prepareTextureForPresenting(target);
	m_context->end();

	CommandListSubmitInfo submitInfo;
	submitInfo.m_fence = m_frameFence;
	m_lastSubmittedFenceValue = m_frameFence->getValue();

	environment->m_graphicDevice->submitCommandList(m_context->getCommandList(), submitInfo);
}

void VT::RenderSystem::waitFrame()
{
	m_frameFence->wait(m_lastSubmittedFenceValue);
}