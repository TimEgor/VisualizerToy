#pragma once

#include "GraphRender/IRenderPass.h"
#include "GraphicResourceManager/ObjectHandles.h"
#include "DefaultRender/DefaultRenderingData.h"

namespace VT
{
	struct RenderDrawingContext;

	class GBufferRenderPassData final : public IRenderPassData
	{
	public:
		using TransformCollection = DefaultRenderingData::TransformDataCollection;
		using MeshCollection = DefaultRenderingData::MeshDataCollection;

	private:
		const TransformCollection& m_transformCollection;
		const MeshCollection& m_meshCollection;

		ShaderResourceViewReference m_cameraTransformBufferView;

	public:
		GBufferRenderPassData(const TransformCollection& transforms, const MeshCollection& meshes,
			const ShaderResourceViewReference& cameraTransformView)
			: m_transformCollection(transforms), m_meshCollection(meshes),
			m_cameraTransformBufferView(cameraTransformView) {}

		const TransformCollection& getTransformData() const { return m_transformCollection; }
		const MeshCollection& getMeshData() const { return m_meshCollection; }

		const ShaderResourceViewReference& getCameraTransformView() const { return m_cameraTransformBufferView; }

		VT_RENDER_PASS_DATA_TYPE(GBUFFER_DATA_TYPE);
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
		PassRenderingData m_passData;
		TestMaterialDrawingData m_materialDrawingData;

		bool initRenderingData();
		bool initMaterial();

	public:
		GBufferPass() = default;

		virtual bool init() override;
		virtual void release() override;

		virtual void fillRenderPassDependency(RenderPassGraphBuilder& builder) const override;

		virtual void execute(RenderDrawingContext& drawContext, const RenderPassEnvironment& environment, const IRenderPassData* data) override;
	};
}
