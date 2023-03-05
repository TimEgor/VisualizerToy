#pragma once

#include "GraphicResourceManager/ObjectHandles.h"
#include "RenderSystem/IRenderPass.h"
#include "RenderSystem/RenderingData.h"

namespace VT
{
	class LightVolumeData;
	class IRenderContext;

	class LightPrepearingPass final : public IRenderPass
	{
		struct SortedPointLight final
		{
			uint16_t m_lightIndex = -1;
			float m_projPosZ = 0.0f;
			float m_projMinPosZ = 0.0f;
			float m_projMaxPosZ = 0.0f;
		};

	private:
		std::vector<SortedPointLight> m_sortedPointLights;

		ComputeShaderReference m_cullingShader;
		PipelineBindingLayoutReference m_bindingLayout;

		const LightVolumeData* m_lightVolumeData = nullptr;

		void sortPointLights(const RenderingData::PointLightDataCollection& data, const CameraTransforms& cameraTransform);
		void fillPointLightBuffer(IGPUBuffer* pointLightBuffer, const RenderingData::PointLightDataCollection& data);
		void fillPointLightZSliceBuffer(IGPUBuffer* zBinsBuffer);

		void computePointLightTileMasks(IRenderContext* context, uint32_t pointLightsCount,
			ShaderResourceViewReference pointLightsSRV, ShaderResourceViewReference tilesUAV,
			ShaderResourceViewReference cameraTransformsSRV, ShaderResourceViewReference lightVolumeSRV);

	public:
		LightPrepearingPass() = default;

		virtual bool init() override;
		virtual void release() override;

		virtual void execute(const RenderPassContext& passContext, const RenderPassEnvironment& environment) override;

		void setLightVolumeData(const LightVolumeData* data) { m_lightVolumeData = data; }
	};
}
