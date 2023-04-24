#pragma once

#include "GraphRender/IRenderPass.h"
#include "GraphicResourceManager/ObjectHandles.h"
#include "MeshSystem/MeshHandle.h"

namespace VT
{
	class IGraphicResourceManager;

	struct MeshVertexData;
	struct MeshIndexData;

	class LightRenderPassData final : public IRenderPassData
	{
	private:
		ShaderResourceViewReference m_cameraTransformBufferView;

	public:
		LightRenderPassData(const ShaderResourceViewReference& cameraTransformView)
			: m_cameraTransformBufferView(cameraTransformView) {}

		const ShaderResourceViewReference& getCameraTransformView() const { return m_cameraTransformBufferView; }

		VT_RENDER_PASS_DATA_TYPE(LIGHT_DATA_TYPE);
	};

	class LightPass final : public IRenderPass
	{
		struct PassPipelineData final
		{
			VertexShaderReference m_presentVertexShader;
			PixelShaderReference m_presentPointLightPixelShader;
			PipelineBindingLayoutReference m_bindingLayout;
		};

	private:
		PassPipelineData m_pipelineData;
		MeshReference m_screenRectGeom;

		bool initPipelineData();
		bool initScreenRect();

		bool initScreenRectVertexData(MeshVertexData& data, IGraphicResourceManager* resManager) const;
		bool initScreenRectIndexData(MeshIndexData& data, IGraphicResourceManager* resManager) const;

	public:
		LightPass() = default;

		virtual bool init() override;
		virtual void release() override;

		virtual void fillRenderPassDependency(RenderPassGraphBuilder& builder) const override;

		virtual void execute(RenderDrawingContext& drawContext, const RenderPassEnvironment& environment, const IRenderPassData* data) override;
	};
}
