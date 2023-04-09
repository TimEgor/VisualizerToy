#include "RenderSystem.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "GraphicDevice/IGraphicDevice.h"
#include "GraphicPlatform/IGraphicPlatform.h"
#include "GraphicResourceManager/IGraphicResourceManager.h"
#include "GraphicRenderContext.h"

#include "Textures/ITexture2D.h"
#include "GPUBuffer/IGPUBuffer.h"

#include "Math/Vector.h"
#include "MeshSystem/IMeshSystem.h"
#include "PreparingRenderingDataSystem.h"
#include "GraphicResourceCommon/IGraphicResourceDescriptor.h"

#include "RenderPasses/GBuffer.h"
#include "RenderPasses/LightingVolumeData.h"

#include "RenderPasses/GBufferPass.h"
#include "RenderPasses/LightPrepearingPass.h"
#include "RenderPasses/LigthingPass.h"
#include "RenderPasses/DebugUiPass.h"
#include "RenderPasses/PresentPass.h"

bool VT::RenderSystem::init()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	ICommandList* commandList = environment->m_graphicDevice->createCommandList();
	VT_CHECK_INITIALIZATION(commandList);

	m_context = environment->m_graphicPlatform->createRenderContext();
	VT_CHECK_INITIALIZATION(m_context && m_context->init(commandList));

	m_frameFence = environment->m_graphicDevice->createFence();
	VT_CHECK_INITIALIZATION(m_frameFence);

	//
	m_renderingData = new RenderingData();
	VT_CHECK_INITIALIZATION(m_renderingData && m_renderingData->init());

	//
	m_passEnvironment = new RenderPassEnvironment();
	VT_CHECK_INITIALIZATION(m_passEnvironment);

	m_gBuffer = new GBuffer();
	VT_CHECK_INITIALIZATION(m_gBuffer && m_gBuffer->init({ 512, 512 }));
	m_gBuffer->fillEnvironment(*m_passEnvironment);

	m_gBufferPass = new GBufferPass();
	VT_CHECK_INITIALIZATION(m_gBufferPass && m_gBufferPass->init());

	m_lightVolume = new LightVolumeData();
	VT_CHECK_INITIALIZATION(m_lightVolume && m_lightVolume->init({ 512, 512 }));
	m_lightVolume->fillEnvironment(*m_passEnvironment);

	m_lightPrepearingPass = new LightPrepearingPass();
	VT_CHECK_INITIALIZATION(m_lightPrepearingPass && m_lightPrepearingPass->init());
	m_lightPrepearingPass->setLightVolumeData(m_lightVolume);

	m_lightPass = new LightPass();
	VT_CHECK_INITIALIZATION(m_lightPass && m_lightPass->init());

	m_debugUiPass = new DebugUiPass();
	VT_CHECK_INITIALIZATION(m_debugUiPass && m_debugUiPass->init());

	//m_presentPass = new PresentPass();
	//VT_CHECK_INITIALIZATION(m_presentPass && m_presentPass->init());

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

	//VT_SAFE_DESTROY_WITH_RELEASING(m_presentPass);

	VT_SAFE_DESTROY_WITH_RELEASING(m_debugUiPass);

	VT_SAFE_DESTROY_WITH_RELEASING(m_lightPass);
	VT_SAFE_DESTROY_WITH_RELEASING(m_lightVolume);

	VT_SAFE_DESTROY_WITH_RELEASING(m_gBufferPass);
	VT_SAFE_DESTROY_WITH_RELEASING(m_gBuffer);
	VT_SAFE_DESTROY(m_passEnvironment);

	VT_SAFE_DESTROY_WITH_RELEASING(m_context);
}

void VT::RenderSystem::collectRenderingData(const ILevel& level)
{
	PreparingRenderingDataSystem::prepareData(level, *m_renderingData);
}

void VT::RenderSystem::render(ITexture2D* target, IGraphicResourceDescriptor* targetView)
{
	assert(m_context);

	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	m_context->begin();

	RenderPassContext renderContext = { m_context, *m_renderingData, target, targetView };

	m_gBufferPass->execute(renderContext, *m_passEnvironment);
	m_lightPrepearingPass->execute(renderContext, *m_passEnvironment);
	m_lightPass->execute(renderContext, *m_passEnvironment);
	m_debugUiPass->execute(renderContext, *m_passEnvironment);
	//m_presentPass->execute(renderContext, *m_passEnvironment);

	GraphicRenderContextUtils::prepareTextureResourceForPresenting(m_context, target);
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