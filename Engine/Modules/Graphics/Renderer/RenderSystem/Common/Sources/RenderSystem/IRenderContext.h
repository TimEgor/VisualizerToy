#pragma once

#include <vector>

namespace VT
{
	class ICommandList;
	class IPipelineState;
	class ITexture2D;
	class ITexture2DView;

	struct RenderContextTarget final
	{
		ITexture2D* m_texture = nullptr;
		ITexture2DView* m_view = nullptr;
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

		virtual void setPipelineState(IPipelineState* pipelineState) = 0;

		virtual void draw() = 0;
	};
}
