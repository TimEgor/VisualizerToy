#pragma once

#include <vector>

#include "Textures/TextureCommon.h"

namespace VT
{
	class IGraphicResourceDescriptor;
	class ICommandList;
	class IPipelineState;
	class IPipelineBindingLayout;
	class ITexture2D;
	class IGPUBuffer;

	struct Scissors final
	{
		TextureDimension m_left = 0;
		TextureDimension m_right = 0;
		TextureDimension m_top = 0;
		TextureDimension m_bottom = 0;

		Scissors() = default;
		Scissors(TextureDimension right, TextureDimension bottom)
			: m_right(right), m_bottom(bottom) {}
	};

	struct Viewport final
	{
		TextureDimension m_width = 0;
		TextureDimension m_height = 0;
		TextureDimension m_x = 0;
		TextureDimension m_y = 0;

		Viewport() = default;
		Viewport(TextureDimension width, TextureDimension height)
			: m_width(width), m_height(height) {}
	};

	struct RenderContextTarget final
	{
		ITexture2D* m_texture = nullptr;
		IGraphicResourceDescriptor* m_view = nullptr;

		Viewport m_viewport;
		Scissors m_scissors;
	};

	struct RenderContextBeginInfo final
	{
		std::vector<RenderContextTarget> m_targets;
	};

	class IRenderContext
	{
	public:
		IRenderContext() = default;
		virtual ~IRenderContext() {}

		virtual bool init(ICommandList* commandList) = 0;
		virtual void release() = 0;

		virtual ICommandList* getCommandList() = 0;

		virtual void begin() = 0;
		virtual void end() = 0;

		virtual void beginRendering(const RenderContextBeginInfo& info) = 0;
		virtual void endRendering() = 0;

		virtual void prepareTextureForRendering(ITexture2D* texture) = 0;
		virtual void prepareTextureForPresenting(ITexture2D* texture) = 0;

		virtual void setPipelineState(const IPipelineState* pipelineState, const IPipelineBindingLayout* bindingLayout) = 0;
		virtual void setVertexBuffers(uint32_t buffersCount, IGPUBuffer** buffers, const InputLayoutDesc& inputLayoutDesc) = 0;
		virtual void setIndexBuffer(IGPUBuffer* buffer, InputLayoutElementType indexType) = 0;

		virtual void draw(uint32_t vertCount) = 0;
		virtual void drawIndexed(uint32_t indexCount) = 0;
	};
}
