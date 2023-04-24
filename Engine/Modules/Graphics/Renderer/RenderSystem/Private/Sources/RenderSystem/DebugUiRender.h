#pragma once

#include "Render/IRender.h"

namespace VT
{
	class DebugUiRender final : public IRender
	{
	public:
		DebugUiRender() = default;

		virtual bool init() override;
		virtual void release() override;

		virtual void draw(RenderDrawingContext& context) override;
	};
}
