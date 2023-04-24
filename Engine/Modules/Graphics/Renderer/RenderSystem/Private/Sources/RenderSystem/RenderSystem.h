#pragma once

#include "RenderSystem/IRenderSystem.h"
#include "GraphicSynchronization/IFence.h"

namespace VT
{
	class IFence;
	class IRenderContext;
	class IPipelineBindingLayout;

	class DebugUiRender;

	class RenderSystem final : public IRenderSystem
	{
	private:
		IRenderContext* m_context = nullptr;

		IFence* m_frameFence = nullptr;
		FenceValueType m_lastSubmittedFenceValue = InitialFenceValue;

		IRender* m_render = nullptr;
		DebugUiRender* m_debugUiRender = nullptr;

	public:
		RenderSystem() = default;
		~RenderSystem() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual void setRender(IRender* render) override;
		virtual void draw(ITexture2D* target, IGraphicResourceDescriptor* targetView) override;

		virtual void waitFrame() override;
	};
}
