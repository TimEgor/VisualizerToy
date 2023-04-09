#pragma once

#include "ImGui/IImGuiBackend.h"
#include "GraphicDevice/IGraphicDevice.h"

namespace VT_D3D12
{
	class D3D12ImGuiRenderBackend final : public VT::IImGuiRenderBackend
	{
	private:
		VT::IGraphicResourceDescriptor* m_fontTextureResourceDescriptor = nullptr;
		bool m_inited = false;

	public:
		D3D12ImGuiRenderBackend() = default;
		virtual ~D3D12ImGuiRenderBackend() { release(); }

		virtual bool init(ImGuiContext* imguiContext) override;
		virtual void release() override;

		virtual void newFrame() override;

		virtual void present(VT::IRenderContext* context) override;
	};
}
