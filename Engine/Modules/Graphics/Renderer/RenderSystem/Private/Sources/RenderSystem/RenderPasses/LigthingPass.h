#pragma once

#include "GraphicResourceManager/ObjectHandles.h"
#include "RenderSystem/IRenderPass.h"

namespace VT
{
	class LightPass final : public IRenderPass
	{
	public:
		LightPass() = default;

		virtual bool init() override;
		virtual void release() override;
		virtual void render(const RenderPassContext& passContext, const RenderPassEnvironment& environment) override;
	};
}