#include "DebugUiRender.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "GraphicDevice/IGraphicDevice.h"

#include "Render/RenderDrawingContext.h"
#include "RenderContext/GraphicRenderContext.h"
#include "RenderContext/RenderContextEvent.h"

#include "DebugUiSystem/IDebugUiSystem.h"
#include "Textures/ITexture2D.h"

bool VT::DebugUiRender::init()
{
	return true;
}

void VT::DebugUiRender::release()
{
}

void VT::DebugUiRender::draw(RenderDrawingContext& context)
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	RenderContextEvent passEvent(context.m_context, "ImGUIPass");

	const Texture2DDesc& targetTextureDesc = context.m_target->getDesc();
	GraphicRenderContextTarget targets[] = {
		{
			context.m_target,
			context.m_targetView,
			Viewport(targetTextureDesc.m_width, targetTextureDesc.m_height),
			Scissors(targetTextureDesc.m_width, targetTextureDesc.m_height)
		}
	};

	GraphicRenderContextUtils::setRenderingTargets(context.m_context, 1, targets);
	context.m_context->setDescriptorHeap(environment->m_graphicDevice->getBindlessResourceDescriptionHeap());

	EngineInstance::getInstance()->getEnvironment()->m_debugUiSystem->render(context.m_context);
}