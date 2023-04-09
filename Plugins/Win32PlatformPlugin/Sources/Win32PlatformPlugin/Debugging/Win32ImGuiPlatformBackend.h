#pragma once

#include "ImGui/IImGuiBackend.h"
#include "EventSystem/IEventSystem.h"

namespace VT_WIN32
{
	struct Win32WindowProcedureEvent;

	class Win32ImGuiPlatformBackend final : public VT::IImGuiPlatformBackend
	{
	private:
		VT::EventDispatcherID m_eventDispatcher = VT::InvalidEventDispatcherID;
		bool m_inited = false;

		void onWindowEvent(const Win32WindowProcedureEvent& event);

	public:
		Win32ImGuiPlatformBackend() = default;
		~Win32ImGuiPlatformBackend() { release(); }

		virtual bool init(ImGuiContext* imguiContext, const VT::IWindow* window) override;
		virtual void release() override;

		virtual void newFrame() override;
	};
}
