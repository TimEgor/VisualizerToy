#include "Win32ImGuiPlatformBackend.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "Win32PlatformPlugin/WindowSystem/Win32WindowEventSystem.h"

#include "WindowSystem/IWindow.h"

#include "backends/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void VT_WIN32::Win32ImGuiPlatformBackend::onWindowEvent(const Win32WindowProcedureEvent& event)
{
	ImGui_ImplWin32_WndProcHandler(event.m_hwnd, event.m_msg, event.m_wParam, event.m_lParam);
}

bool VT_WIN32::Win32ImGuiPlatformBackend::init(ImGuiContext* imguiContext, const VT::IWindow* window)
{
	assert(!m_inited);

	VT_CHECK_INITIALIZATION(imguiContext && window);

	VT::IEventSystem* eventSystem = VT::EngineInstance::getInstance()->getEnvironment()->m_eventSystem;
	m_eventDispatcher = eventSystem->addEventDispatcher(Win32WindowProcedureEvent::getEventType(),
		[this](const VT::IEvent& event) { onWindowEvent(reinterpret_cast<const Win32WindowProcedureEvent&>(event)); }
	);

	ImGui::SetCurrentContext(imguiContext);

	VT_CHECK_INITIALIZATION(m_eventDispatcher != VT::InvalidEventDispatcherID);
	VT_CHECK_INITIALIZATION(ImGui_ImplWin32_Init(window->getNativeHandle()));

	m_inited = true;

	return true;
}

void VT_WIN32::Win32ImGuiPlatformBackend::release()
{
	if (m_eventDispatcher != VT::InvalidEventDispatcherID)
	{
		VT::IEventSystem* eventSystem = VT::EngineInstance::getInstance()->getEnvironment()->m_eventSystem;
		eventSystem->removeEventDispatcher(m_eventDispatcher);
	}


	if (m_inited)
	{
		ImGui_ImplWin32_Shutdown();
		m_inited = false;
	}
}

void VT_WIN32::Win32ImGuiPlatformBackend::newFrame()
{
	assert(m_inited);

	ImGui_ImplWin32_NewFrame();
}
