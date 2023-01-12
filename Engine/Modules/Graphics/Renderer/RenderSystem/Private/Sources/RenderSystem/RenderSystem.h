#pragma once

#include "RenderSystem/IRenderSystem.h"
#include "GraphicSynchronization/IFence.h"
#include "GraphicResourceManager/ResourceHandles.h"
#include "MeshSystem/MeshHandle.h"

namespace VT
{
	class IFence;
	class IRenderContext;
	class IPipelineBindingLayout;

	struct TestDrawingData final
	{
		VertexShaderReference m_vertShader;
		PixelShaderReference m_pixelShader;
		MeshReference m_mesh;
	};

	struct DrawingPassData final
	{
		GPUBufferReference m_dynamicTransformData; // Global camera and per object transforms
		IPipelineBindingLayout* m_bindingLayout = nullptr;
	};

	class RenderSystem final : public IRenderSystem
	{
	private:
		IRenderContext* m_context = nullptr;

		IFence* m_frameFence = nullptr;
		FenceValueType m_lastSubmittedFenceValue = 0;

		TestDrawingData m_drawingData;
		DrawingPassData m_drawingPassData;

	public:
		RenderSystem() = default;
		~RenderSystem() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual void render(ITexture2D* target, IGraphicResourceDescriptor* targetView) override;

		virtual void waitFrame() override;
	};
}
