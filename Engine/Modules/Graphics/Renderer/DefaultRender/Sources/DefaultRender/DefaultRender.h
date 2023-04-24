#pragma once

#include "DefaultRenderingData.h"
#include "GraphRender/GraphRenderBase.h"

namespace VT
{
	class GBuffer;
	class GBufferPass;
	class GBufferRenderPassData;

	class LightVolumeData;
	class LightPrepearingPass;
	class LightPrepearingRenderPassData;
	class LightPass;
	class LightRenderPassData;

	class PreparingRenderingDataSystem;

	class DefaultRender : public GraphRenderBase
	{
		struct RenderPassData final
		{
			GBuffer* m_gBuffer = nullptr;
			LightVolumeData* m_lightVolumeData = nullptr;

			bool init();
			void release();

			void fillRenderPassEnvironment(RenderPassEnvironment& environment);
		};

		struct RenderPasses final
		{
			GBufferPass* m_gBufferPass = nullptr;
			LightPrepearingPass* m_lightPrepearingPass = nullptr;
			LightPass* m_lightPass = nullptr;

			GraphPassID m_gBufferPassID = InvalidGraphPassID;
			GraphPassID m_lightPrepearingPassID = InvalidGraphPassID;
			GraphPassID m_lightPassID = InvalidGraphPassID;
		};

		struct RenderPassDatas final
		{
			GBufferRenderPassData* m_gBufferPassData = nullptr;
			LightPrepearingRenderPassData* m_lightPrepearingPassData = nullptr;
			LightRenderPassData* m_lightRenderPassData = nullptr;
		};

	private:
		DefaultRenderingData m_renderingData;

		RenderPassData m_renderPassData;
		RenderPasses m_passes;
		RenderPassDatas m_passDatas;

	protected:
		virtual bool fillRenderGraphInfo(RenderPassGraphBuilder& builder);

	public:
		DefaultRender() = default;
		~DefaultRender() { release(); }

		virtual bool init() override;
		virtual void release() override;

		DefaultRenderingData& getDefaultRenderingData() { return m_renderingData; }
	};
}
