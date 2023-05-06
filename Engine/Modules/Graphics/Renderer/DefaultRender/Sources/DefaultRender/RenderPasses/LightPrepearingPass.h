#pragma once

#include "GraphRender/IRenderPass.h"
#include "GraphicResourceManager/ObjectHandles.h"
#include "DefaultRender/DefaultRenderingData.h"

namespace VT
{
	class IGraphicResourceManager;
	class LightVolumeData;
	class IRenderContext;

	class LightPrepearingRenderPassData final : public IRenderPassData
	{
	public:
		using PointLightCollection = DefaultRenderingData::PointLightDataCollection;
		using DirectionalLightCollection = DefaultRenderingData::DirectionalLightDataCollection;

	private:
		const PointLightCollection& m_pointLightCollection;
		const DirectionalLightCollection& m_directionalLightCollection;
		const CameraTransforms& m_cameraTransform;
		ShaderResourceViewReference m_cameraTransformBufferView;

		Vector2UInt16 m_tilesCount = Vector2UInt16Zero;
		uint16_t m_slicesCount = 0;

	public:
		LightPrepearingRenderPassData(const Vector2UInt16& tilesCount, uint16_t slicesCount,
			const PointLightCollection& pointLights, const DirectionalLightCollection& dirLights,
			const CameraTransforms& cameraTransform, const ShaderResourceViewReference& cameraTransformView)
			: m_tilesCount(tilesCount), m_slicesCount(slicesCount),
			m_pointLightCollection(pointLights), m_directionalLightCollection(dirLights),
			m_cameraTransform(cameraTransform), m_cameraTransformBufferView(cameraTransformView) {}

		const PointLightCollection& getPointLightData() const { return m_pointLightCollection; }
		const DirectionalLightCollection& getDirectionalLightDatas() const { return m_directionalLightCollection; }

		const CameraTransforms& getCameraTransform() const { return m_cameraTransform; }
		const ShaderResourceViewReference& getCameraTransformView() const { return m_cameraTransformBufferView; }

		const Vector2UInt16& getTilesCount() const { return m_tilesCount; }
		uint16_t getSlicesCount() const { return m_slicesCount; }

		VT_RENDER_PASS_DATA_TYPE(LIGHT_PREPEARING_DATA_TYPE);
	};

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

		ComputeShaderReference m_pointLightMaskClearingShader;
		ComputeShaderReference m_pointLightCullingShader;

		ComputeShaderReference m_lightVolumeDepthCalcShader;
		ComputeShaderReference m_lightVolumeBBCalcShader;

		PipelineBindingLayoutReference m_pointLightMaskClearingBindingLayout;
		PipelineBindingLayoutReference m_pointLightCullingBindingLayout;

		PipelineBindingLayoutReference m_lightVolumeDepthCalcBindingLayout;
		PipelineBindingLayoutReference m_lightVolumeBBCalcBindingLayout;

		bool initPointLightMaskClearingData(IGraphicResourceManager* resManager);
		bool initPointLightCullingData(IGraphicResourceManager* resManager);

		bool initDepthCalcData(IGraphicResourceManager* resManager);
		bool initBoundingBoxCalcData(IGraphicResourceManager* resManager);

		void sortPointLights(const LightPrepearingRenderPassData::PointLightCollection& data, const CameraTransforms& cameraTransform);
		void fillPointLightBuffer(IGPUBuffer* pointLightBuffer, const LightPrepearingRenderPassData::PointLightCollection& data);
		void fillPointLightZSliceBuffer(IGPUBuffer* zBinsBuffer, uint16_t slicesCount);

		void fillDirLightBuffer(IGPUBuffer* dirLightBuffer, const LightPrepearingRenderPassData::DirectionalLightCollection& data);

		void computeTilesDepth(IRenderContext* context, ShaderResourceViewReference tilesDepthUAV,
			ShaderResourceViewReference lightVolumeSRV, ShaderResourceViewReference depthSourceSRV, ITexture2D* depthSource);
		void computeTilesBoudingBoxes(IRenderContext* context, ShaderResourceViewReference tilesBbUAV, ShaderResourceViewReference tilesDepthSRV,
			ShaderResourceViewReference cameraTransformsSRV, ShaderResourceViewReference lightVolumeSRV, const Vector2UInt16& tilesCount);

		void clearPointLightTileMasks(IRenderContext* context, ShaderResourceViewReference tilesUAV,
			ShaderResourceViewReference lightVolumeSRV, const Vector2UInt16& tilesCount);

		void computePointLightTileMasks(IRenderContext* context, uint32_t pointLightsCount,
			ShaderResourceViewReference pointLightsSRV, ShaderResourceViewReference tilesUAV, ShaderResourceViewReference tilesBbSRV,
			ShaderResourceViewReference cameraTransformsSRV, ShaderResourceViewReference lightVolumeSRV);

	public:
		LightPrepearingPass() = default;

		virtual bool init() override;
		virtual void release() override;

		virtual void fillRenderPassDependency(RenderPassGraphBuilder& builder) const override;

		virtual void execute(RenderDrawingContext& drawContext, const RenderPassEnvironment& environment, const IRenderPassData* data) override;
	};
}
