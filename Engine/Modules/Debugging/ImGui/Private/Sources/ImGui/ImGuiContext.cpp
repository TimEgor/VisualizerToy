#include "ImGuiContext.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "Core/UtilitiesMacros.h"
#include "ImGui/IImGuiBackend.h"

#include "Platform/IPlatform.h"
#include "GraphicPlatform/IGraphicPlatform.h"

#include "imgui.h"

bool VT::ImGuiContext::init(IWindow* window)
{

	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();


	m_platformBackend = environment->m_platform->createImGuiPlatformBackend();
	m_renderBackend = environment->m_graphicPlatform->createImGuiRenderBackend();

	::ImGuiContext* imguiContext = ImGui::CreateContext();

	VT_CHECK_INITIALIZATION(m_platformBackend && m_platformBackend->init(imguiContext, window));
	VT_CHECK_INITIALIZATION(m_renderBackend && m_renderBackend->init(imguiContext));

	return true;
}

void VT::ImGuiContext::release()
{
	VT_SAFE_DESTROY_WITH_RELEASING(m_renderBackend);
	VT_SAFE_DESTROY_WITH_RELEASING(m_platformBackend);

	ImGui::DestroyContext();
}

void VT::ImGuiContext::update()
{
	m_platformBackend->newFrame();
	m_renderBackend->newFrame();

	ImGui::NewFrame();

	ImGui::ShowDemoWindow();
}

void VT::ImGuiContext::render(IRenderContext* context)
{
	ImGui::Render();

	m_renderBackend->present(context);
}