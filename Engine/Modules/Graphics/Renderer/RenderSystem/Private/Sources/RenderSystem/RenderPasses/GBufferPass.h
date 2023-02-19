#pragma once

#include "GraphicResourceManager/ObjectHandles.h"
#include "RenderSystem/IRenderPass.h"

namespace VT
{
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
		PassRenderingData m_passData;
		TestMaterialDrawingData m_materialDrawingData;

		bool initRenderingData();
		bool initMaterial();

	public:
		GBufferPass() = default;

		virtual bool init() override;
		virtual void release() override;
		virtual void render(const RenderPassContext& passContext, const RenderPassEnvironment& environment) override;
	};
}
