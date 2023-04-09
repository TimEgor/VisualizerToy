#include "DebugUiSystem.h"

#include "Core/UtilitiesMacros.h"
#include "ImGui/ImGuiContext.h"

bool VT::DebugUiSystem::init()
{
	return true;
}

void VT::DebugUiSystem::release()
{
	VT_SAFE_DESTROY_WITH_RELEASING(m_context);
}

bool VT::DebugUiSystem::setWindow(IWindow* window)
{
	m_context = new ImGuiContext();
	if (!m_context->init(window))
	{
		VT_SAFE_DESTROY_WITH_RELEASING(m_context);

		return false;
	}

	return true;
}

void VT::DebugUiSystem::update()
{
	if (m_context)
	{
		m_context->update();
	}
}

void VT::DebugUiSystem::render(IRenderContext* context)
{
	if (m_context)
	{
		m_context->render(context);
	}
}
