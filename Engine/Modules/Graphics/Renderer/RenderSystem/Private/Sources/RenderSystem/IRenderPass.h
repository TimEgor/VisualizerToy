#pragma once

namespace VT
{
	class IRenderContext;
	class RenderingData;
	class RenderPassEnvironment;

	struct RenderPassContext final
	{
		IRenderContext* context = nullptr;
		const RenderingData& m_renderingData;
	};

	class IRenderPass
	{
	public:
		IRenderPass() = default;
		virtual ~IRenderPass() = default;

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual void render(const RenderPassContext& passContext, const RenderPassEnvironment& environment) = 0;
	};
}
