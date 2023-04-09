#pragma once

#include "ImGui/IImGuiContext.h"

namespace VT
{
	class IImGuiRenderBackend;
	class IImGuiPlatformBackend;
	class ITexture2D;
	class IGraphicResourceDescriptor;

	class ImGuiContext final : public IImGuiContext
	{
	private:
		IImGuiPlatformBackend* m_platformBackend = nullptr;
		IImGuiRenderBackend* m_renderBackend = nullptr;

	public:
		ImGuiContext() = default;

		virtual bool init(IWindow* window) override;
		virtual void release() override;

		virtual void update() override;

		virtual void render(IRenderContext* context) override;
	};
}
