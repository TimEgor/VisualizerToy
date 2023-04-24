#pragma once

#include "GraphicResourceManager/ObjectHandles.h"
#include "MeshSystem/MeshHandle.h"
#include "GraphRender/IRenderPass.h"

namespace VT
{
	class IGraphicResourceManager;
	struct MeshVertexData;
	struct MeshIndexData;

	class PresentPass final : public IRenderPass
	{
		struct PassPipelineData final
		{
			VertexShaderReference m_presentVertexShader;
			PixelShaderReference m_presentPixelShader;
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
		PresentPass() = default;

		virtual bool init() override;
		virtual void release() override;
		virtual void execute(RenderDrawingContext& drawContext, const RenderPassEnvironment& environment, const IRenderPassData* data) override;
	};
}
