#pragma once

#include "Render/IRender.h"
#include "RenderPassGraph.h"

namespace VT
{
	class IRenderPassData;

	class GraphRenderBase : public IRender
	{
		using RenderPassDataContainer = std::unordered_map<GraphPassID, IRenderPassData*>;

	private:
		RenderPassGraph m_renderGraph;
		RenderPassEnvironment* m_environments = nullptr;

		RenderPassDataContainer m_passDatas;

		uint32_t m_environmentIndex = 0;
		uint32_t m_environmentsCount = 0;

	protected:
		bool initRenderGraph(const RenderPassGraphBuilder& builder);
		void clearRenderGraph();

		bool initEnvironments(uint32_t count);
		void releaseEnvironments();

		uint32_t getEnvironmentCount() const { return m_environmentsCount; }
		RenderPassEnvironment& getEnvironment(uint32_t index);

	public:
		GraphRenderBase() = default;

		void addRenderPassData(GraphPassID passID, IRenderPassData* data);
		void removeRenderPassData(GraphPassID passID);
		IRenderPassData* getRenderPassData(GraphPassID passID);

		virtual void draw(RenderDrawingContext& context) override;
	};
}
