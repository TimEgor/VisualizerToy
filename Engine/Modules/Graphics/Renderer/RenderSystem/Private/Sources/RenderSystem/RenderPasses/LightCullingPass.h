#pragma once

#include "GraphicResourceManager/ObjectHandles.h"
#include "RenderSystem/IRenderPass.h"

namespace VT
{
	class LightPassCulling final : public IRenderPass
	{
	public:
		LightPassCulling() = default;

		virtual bool init() override;
		virtual void release() override;
		virtual void render(const RenderPassContext& passContext, const RenderPassEnvironment& environment) override;
	};
}