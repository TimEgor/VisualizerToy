#pragma once

#include "RenderSystem/IRenderSystem.h"
#include "GraphicSynchronization/IFence.h"

#include "RenderPassEnvironment.h"
#include "RenderingData.h"
#include "RenderPasses/GBufferPass.h"

namespace VT
{
	class IFence;
	class IRenderContext;
	class IPipelineBindingLayout;

	struct GBuffer;

	class RenderSystem final : public IRenderSystem
	{
	private:
		IRenderContext* m_context = nullptr;

		IFence* m_frameFence = nullptr;
		FenceValueType m_lastSubmittedFenceValue = InitialFenceValue;

		CameraRenderingData m_cameraData;
		RenderingData m_renderingData;

		RenderPassEnvironment* m_passEnvironment = nullptr;

		GBuffer* m_gBuffer = nullptr;
		GBufferPass* m_gBufferPass = nullptr;

		bool initCameraData();

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
