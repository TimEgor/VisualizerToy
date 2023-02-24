#pragma once

#include "GraphicResourceManager/ObjectHandles.h"
#include "RenderSystem/IRenderPass.h"
#include "Math/Vector.h"

namespace VT
{
	class LightBuffer final
	{
	private:
		Texture2DReference m_lightTexture;
		GraphicResourceReference m_lightTextureUAV;

	public:
		LightBuffer() = default;

		bool init(const Vector2UInt16& bufferResolution);
		void release();

		void fillEnvironment(RenderPassEnvironment& environment) const;
	};

	class LightPass final : public IRenderPass
	{
	private:
		ComputeShaderReference m_presenterShader;
		PipelineBindingLayoutReference m_bindingLayout;

	public:
		LightPass() = default;

		virtual bool init() override;
		virtual void release() override;
		virtual void execute(const RenderPassContext& passContext, const RenderPassEnvironment& environment) override;
	};
}