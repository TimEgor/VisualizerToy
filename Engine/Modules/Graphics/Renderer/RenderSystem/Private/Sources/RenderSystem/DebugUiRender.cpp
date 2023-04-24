#include "DebugUiRender.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "Render/RenderDrawingContext.h"

#include "DebugUiSystem/IDebugUiSystem.h"

bool VT::DebugUiRender::init()
{
	return true;
}

void VT::DebugUiRender::release()
{
}

void VT::DebugUiRender::draw(RenderDrawingContext& context)
{
	EngineInstance::getInstance()->getEnvironment()->m_debugUiSystem->render(context.m_context);
}