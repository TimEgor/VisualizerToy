#include "RenderSystem.h"

#include "DebugUiRender.h"
#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "GraphicDevice/IGraphicDevice.h"
#include "GraphicResourceCommon/IGraphicResourceDescriptor.h"
#include "GraphicPlatform/IGraphicPlatform.h"

#include "RenderContext/GraphicRenderContext.h"
#include "Render/RenderDrawingContext.h"

#include "Textures/ITexture2D.h"

#include "MeshSystem/IMeshSystem.h"
#include "Profile/Profile.h"
#include "Render/IRender.h"

bool VT::RenderSystem::init()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	ICommandList* commandList = environment->m_graphicDevice->createCommandList();
	VT_CHECK_INITIALIZATION(commandList);

	m_context = environment->m_graphicPlatform->createRenderContext();
	VT_CHECK_INITIALIZATION(m_context && m_context->init(commandList));

	m_frameFence = environment->m_graphicDevice->createFence();
	VT_CHECK_INITIALIZATION(m_frameFence);

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

void VT::RenderSystem::setRender(IRender* render)
{
	assert(render);
	m_render = render;
}

void VT::RenderSystem::draw(ITexture2D* target, IGraphicResourceDescriptor* targetView)
{
	assert(m_context);
	assert(m_render);

	VT_PROFILE_BLOCK_EVENT("RenderSystem drawing");

	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	RenderDrawingContext drawingContext = { m_context, target, targetView };

	m_context->begin();

	m_render->draw(drawingContext);
	m_debugUiRender->draw(drawingContext);

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