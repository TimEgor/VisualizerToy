#pragma once

#include "RenderSystem/IRenderSystem.h"
#include "GraphicSynchronization/IFence.h"

#include "RenderPassEnvironment.h"
#include "RenderingData.h"

namespace VT
{
	class IFence;
	class IRenderContext;
	class IPipelineBindingLayout;

	class GBuffer;
	class LightVolumeData;

	class GBufferPass;
	class LightPrepearingPass;
	class LightPass;
	class PresentPass;

	class RenderSystem final : public IRenderSystem
	{
	private:
		IRenderContext* m_context = nullptr;

		IFence* m_frameFence = nullptr;
		FenceValueType m_lastSubmittedFenceValue = InitialFenceValue;

		RenderingData* m_renderingData;

		RenderPassEnvironment* m_passEnvironment = nullptr;

		GBuffer* m_gBuffer = nullptr;
		LightVolumeData* m_lightVolume = nullptr;

		GBufferPass* m_gBufferPass = nullptr;
		LightPrepearingPass* m_lightPrepearingPass = nullptr;
		LightPass* m_lightPass = nullptr;

	public:
		RenderSystem() = default;
		~RenderSystem() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual void collectRenderingData(const ILevel& level) override;
		virtual void render(ITexture2D* target, IGraphicResourceDescriptor* targetView) override;

		virtual void waitFrame() override;
	};
}
