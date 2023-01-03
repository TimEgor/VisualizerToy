#pragma once

#include "RenderSystem/IRenderSystem.h"
#include "GraphicResourceManager/InputLayoutHandle.h"
#include "GraphicResourceManager/ResourceHandles.h"

namespace VT
{
	class IFence;
	class ICommandPool;
	class IRenderContext;

	struct TestDrawingData final
	{
		VertexShaderReference m_vertShader;
		PixelShaderReference m_pixelShader;
		InputLayoutReference m_inputLayout;
		GPUBufferReference m_vertexBuffer;
		GPUBufferReference m_indexBuffer;
	};

	class RenderSystem final : public IRenderSystem
	{
	private:
		IRenderContext* m_context = nullptr;
		ICommandPool* m_commandPool = nullptr;

		IFence* m_frameFence = nullptr;
		ISemaphore* m_renderingCompleteSemaphore = nullptr;

		TestDrawingData m_drawingData;

	public:
		RenderSystem() = default;
		~RenderSystem() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual void render(ITexture2D* target, ITexture2DView* targetView,
			ISemaphore* textureAvailableSemaphore) override;

		virtual void waitFrame() override;

		virtual ISemaphore* getRenderCompletedSemaphore() override;
	};
}
