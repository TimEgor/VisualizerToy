#pragma once

#include "GraphicResourceManager/ObjectHandles.h"
#include "RenderSystem/IRenderPass.h"
#include "RenderSystem/RenderingData.h"

namespace VT
{
	class IGraphicResourceManager;
	class LightVolumeData;
	class IRenderContext;

	class LightPrepearingPass final : public IRenderPass
	{
		struct SortedPointLight final
		{
			uint32_t m_lightIndex = -1;
			float m_projPosZ = 0.0f;
			float m_projMinPosZ = 0.0f;
			float m_projMaxPosZ = 0.0f;
		};

	private:
		std::vector<SortedPointLight> m_sortedPointLights;

		ComputeShaderReference m_pointLightCullingShader;

		ComputeShaderReference m_lightVolumeDepthCalcShader;
		ComputeShaderReference m_lightVolumeBBCalcShader;

		PipelineBindingLayoutReference m_cullingBindingLayout;

		PipelineBindingLayoutReference m_lightVolumeDepthCalcBindingLayout;
		PipelineBindingLayoutReference m_lightVolumeBBCalcBindingLayout;

		const LightVolumeData* m_lightVolumeData = nullptr;

		bool initPointLightCullingData(IGraphicResourceManager* resManager);

		bool initDepthCalcData(IGraphicResourceManager* resManager);
		bool initBoundingBoxCalcData(IGraphicResourceManager* resManager);

		void sortPointLights(const RenderingData::PointLightDataCollection& data, const CameraTransforms& cameraTransform);
		void fillPointLightBuffer(IGPUBuffer* pointLightBuffer, const RenderingData::PointLightDataCollection& data);
		void fillPointLightZSliceBuffer(IGPUBuffer* zBinsBuffer);

		void computeTilesDepth(IRenderContext* context, ShaderResourceViewReference tilesDepthUAV,
			ShaderResourceViewReference lightVolumeSRV, ShaderResourceViewReference depthSourceSRV, ITexture2D* depthSource);
		void computeTilesBoudingBoxes(IRenderContext* context, ShaderResourceViewReference tilesBbUAV,
			ShaderResourceViewReference cameraTransformsSRV, ShaderResourceViewReference lightVolumeSRV);

		void computePointLightTileMasks(IRenderContext* context, uint32_t pointLightsCount,
			ShaderResourceViewReference pointLightsSRV, ShaderResourceViewReference tilesUAV, ShaderResourceViewReference tilesBbSRV,
			ShaderResourceViewReference cameraTransformsSRV, ShaderResourceViewReference lightVolumeSRV);

	public:
		LightPrepearingPass() = default;

		virtual bool init() override;
		virtual void release() override;

		virtual void execute(const RenderPassContext& passContext, const RenderPassEnvironment& environment) override;

		void setLightVolumeData(const LightVolumeData* data) { m_lightVolumeData = data; }
	};
}
