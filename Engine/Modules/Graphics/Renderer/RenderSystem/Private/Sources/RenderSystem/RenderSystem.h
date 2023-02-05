#pragma once

#include "GraphicRenderContext.h"
#include "RenderSystem/IRenderSystem.h"
#include "GraphicSynchronization/IFence.h"
#include "GraphicResourceManager/ObjectHandles.h"
#include "MeshSystem/MeshHandle.h"
#include "RenderingData.h"

namespace VT
{
	class IFence;
	class IRenderContext;
	class IPipelineBindingLayout;

	struct TestMaterialDrawingData final
	{
		VertexShaderReference m_vertShader;
		PixelShaderReference m_pixelShader;
	};

	struct alignas(16) CameraTransforms final
	{
		Matrix44 m_viewTransform = Matrix44Identity;
		Matrix44 m_projectionTransform = Matrix44Identity;
	};

	struct DrawingPassData final
	{
		GPUBufferReference m_cameraTransformBuffer; // Global camera transforms
		GPUBufferReference m_perObjectTransformBuffer;
		ShaderResourceViewReference m_cameraTransformCBV;
		ShaderResourceViewReference m_perObjectTransformSRV;
		PipelineBindingLayoutReference m_bindingLayout;
	};

	class RenderSystem final : public IRenderSystem
	{
	private:
		IRenderContext* m_context = nullptr;
		GraphicRenderContext m_graphicContext;

		IFence* m_frameFence = nullptr;
		FenceValueType m_lastSubmittedFenceValue = InitialFenceValue;

		TestMaterialDrawingData m_materialDrawingData;
		DrawingPassData m_drawingPassData;
		RenderingData m_renderingData;

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
