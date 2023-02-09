#pragma once

#include "GraphicResourceManager/ObjectHandles.h"
#include "RenderSystem/IRenderPass.h"
#include "Math/Vector.h"

namespace VT
{
	struct GBuffer final
	{
		Texture2DReference m_color;
		Texture2DReference m_normal;
		Texture2DReference m_position;
		Texture2DReference m_depth;

		RenderTargetViewReference m_colorRTV;
		RenderTargetViewReference m_normalRTV;
		RenderTargetViewReference m_positionRTV;
		DepthStencilViewReference m_depthDSV;

		ShaderResourceViewReference m_colorSRV;
		ShaderResourceViewReference m_normalSRV;
		ShaderResourceViewReference m_positionSRV;
		ShaderResourceViewReference m_depthSRV;

		bool init(const Vector2UInt16& bufferResolution);
		void release();
	};

	class GBufferPass final : public IRenderPass
	{
		struct PassRenderingData final
		{
			GPUBufferReference m_perObjectTransformBuffer;
			ShaderResourceViewReference m_perObjectTransformSRV;
			PipelineBindingLayoutReference m_bindingLayout; //TMP
		};

		struct TestMaterialDrawingData final
		{
			VertexShaderReference m_vertShader;
			PixelShaderReference m_pixelShader;
		};

	private:
		GBuffer m_gBuffer;
		PassRenderingData m_passData;
		TestMaterialDrawingData m_materialDrawingData;

		bool initRenderingData();
		bool initMaterial();

	public:
		GBufferPass() = default;

		virtual bool init() override;
		virtual void release() override;
		virtual void render(const RenderPassContext& passContext) override;

		const GBuffer& getGPUBuffer() const { return m_gBuffer; }
	};
}
