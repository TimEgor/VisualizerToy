#pragma once

namespace VT
{
	struct RenderDrawingContext;
	class IRenderPassSequence;

	class IRender
	{
	public:
		IRender() = default;
		virtual ~IRender() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual void draw(RenderDrawingContext& context) = 0;
	};
}
