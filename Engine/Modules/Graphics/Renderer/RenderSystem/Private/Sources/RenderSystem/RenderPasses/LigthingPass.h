#pragma once

#include "GraphicResourceManager/ObjectHandles.h"
#include "RenderSystem/IRenderPass.h"
#include "MeshSystem/MeshHandle.h"

namespace VT
{
	class IGraphicResourceManager;

	struct MeshVertexData;
	struct MeshIndexData;

	class LightPass final : public IRenderPass
	{
		struct PassPipelineData final
		{
			VertexShaderReference m_presentVertexShader;
			PixelShaderReference m_presentPointLightPixelShader;
			PipelineBindingLayoutReference m_bindingLayout;
		};

	private:
		PassPipelineData m_pipelineData;
		MeshReference m_screenRectGeom;

		bool initPipelineData();
		bool initScreenRect();

		bool initScreenRectVertexData(MeshVertexData& data, IGraphicResourceManager* resManager) const;
		bool initScreenRectIndexData(MeshIndexData& data, IGraphicResourceManager* resManager) const;

	public:
		LightPass() = default;

		virtual bool init() override;
		virtual void release() override;
		virtual void execute(const RenderPassContext& passContext, const RenderPassEnvironment& environment) override;
	};
}
