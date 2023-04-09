#include "DebugUiPass.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "DebugUiSystem/IDebugUiSystem.h"

bool VT::DebugUiPass::init()
{
	return true;
}

void VT::DebugUiPass::release()
{
}

void VT::DebugUiPass::execute(const RenderPassContext& passContext, const RenderPassEnvironment& environment)
{
	EngineInstance::getInstance()->getEnvironment()->m_debugUiSystem->render(passContext.m_context);
}
