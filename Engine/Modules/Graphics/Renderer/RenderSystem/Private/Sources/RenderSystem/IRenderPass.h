#pragma once

namespace VT
{
	class IRenderContext;
	class RenderingData;
	class RenderPassEnvironment;

	struct RenderPassContext final
	{
		IRenderContext* m_context = nullptr;
		const RenderingData& m_renderingData;

		ITexture2D* m_target = nullptr;
		IGraphicResourceDescriptor* m_targetView = nullptr;
	};

	class IRenderPass
	{
	public:
		IRenderPass() = default;
		virtual ~IRenderPass() = default;

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual void execute(const RenderPassContext& passContext, const RenderPassEnvironment& environment) = 0;
	};
}
