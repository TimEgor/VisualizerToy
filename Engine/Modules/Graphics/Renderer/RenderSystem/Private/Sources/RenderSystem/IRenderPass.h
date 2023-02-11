#pragma once

#include "GraphicResourceManager/ObjectHandles.h"

namespace VT
{
	class IRenderContext;
	class RenderingData;
	class RenderPassEnvironment;

	struct CameraRenderingData final
	{
		GPUBufferReference m_cameraTransformBuffer; // Global camera transforms
		ShaderResourceViewReference m_cameraTransformCBV;
	};

	struct RenderPassContext final
	{
		IRenderContext* context = nullptr;

		const RenderingData& m_renderingData;
		const CameraRenderingData& m_cameraData;
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
