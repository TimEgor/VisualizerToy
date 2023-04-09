#pragma once

#include "RenderSystem/IRenderPass.h"

namespace VT
{
	class DebugUiPass final : public IRenderPass
	{
	public:
		DebugUiPass() = default;

		virtual bool init() override;
		virtual void release() override;

		virtual void execute(const RenderPassContext& passContext, const RenderPassEnvironment& environment) override;
	};
}
