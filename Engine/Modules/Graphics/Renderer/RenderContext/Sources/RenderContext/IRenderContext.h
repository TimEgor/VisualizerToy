#pragma once

#include "Textures/TextureCommon.h"
#include "GraphicResourceCommon/PrimitiveTopology.h"
#include "InputLayout/InputLayout.h"

namespace VT
{
	class IGraphicResourceDescriptor;
	class IGraphicResourceDescriptorHeap;
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

	class IRenderContext
	{
	public:
		static constexpr uint32_t MAX_RENDER_TARGETS_COUNT = 8;
		static constexpr uint32_t MAX_VERTEX_BUFFERS_COUNT = 16;

	public:
		IRenderContext() = default;
		virtual ~IRenderContext() {}

		virtual bool init(ICommandList* commandList) = 0;
		virtual void release() = 0;

		virtual ICommandList* getCommandList() = 0;

		virtual void begin() = 0;
		virtual void end() = 0;

		virtual void clearRenderTarget(const IGraphicResourceDescriptor* renderTargetView, const float* clearValues) = 0;
		virtual void setRenderTargets(uint32_t count, IGraphicResourceDescriptor* const* renderTargetViews) = 0;
		virtual void setViewports(uint32_t count, const Viewport* viewports) = 0;
		virtual void setScissors(uint32_t count, const Scissors* scissors) = 0;

		virtual void changeResourceState(IGraphicResource* resource,
			GraphicStateValueType currentState, GraphicStateValueType targetState) = 0;

		virtual void setDescriptorHeap(IGraphicResourceDescriptorHeap* heap) = 0;

		virtual void setBindingParameterValue(uint32_t index, uint32_t offset, uint32_t value) = 0;
		virtual void setBindingParameterValues(uint32_t index, uint32_t offset, uint32_t valuesCount, uint32_t* values) = 0;
		virtual void setBindingLayout(const IPipelineBindingLayout* bindingLayout) = 0;

		virtual void setPipelineState(const IPipelineState* pipelineState) = 0;
		virtual void setVertexBuffers(uint32_t buffersCount, IGPUBuffer** buffers, const InputLayoutDesc& inputLayoutDesc) = 0;
		virtual void setIndexBuffer(IGPUBuffer* buffer, InputLayoutElementType indexType) = 0;

		virtual void setPrimitiveTopology(PrimitiveTopology topology) = 0;

		virtual void draw(uint32_t vertCount) = 0;
		virtual void drawIndexed(uint32_t indexCount) = 0;
	};
}
