#pragma once

#include "RenderPassEnvironment.h"
#include "RenderPassGraph.h"

#include <vector>

namespace VT
{
	class IRenderPass;

	class RenderPassGraphBuilder final
	{
		using RenderPassContainer = std::vector<RenderPassGraph::RenderPass>;
		using RenderPassResourceDependencyContainer = std::vector<RenderPassGraph::LayerResoureInfo>;
		using DependenciesContainer = std::vector<RenderPassResourceDependencyContainer>;
		using RenderPassIndexMap = std::unordered_map<IRenderPass*, size_t>;

		using PassIndexContainer = std::vector<size_t>;
		using DependenciesPassIndexCollection = std::vector<PassIndexContainer>;

		struct BuildingContext final
		{
			DependenciesPassIndexCollection m_passesDependencies;
			PassIndexContainer m_passDepth;

			void clear();
		};

	private:
		RenderPassIndexMap m_passesIndices;

		RenderPassContainer m_passes;
		DependenciesContainer m_writingResources;
		DependenciesContainer m_readingResources;

		mutable BuildingContext m_buildContext;

		GraphPassID m_passID = 0;

		bool isPassDependent(size_t checkingPassIndex, size_t parentCheckingPassIndex) const;
		void prepareDependentPasses() const;
		size_t preparePassLayerDepth() const;

		GraphPassID getNextPassID();

	public:
		RenderPassGraphBuilder() = default;

		GraphPassID addRenderPass(IRenderPass* pass);
		void addRenderPassReadResource(const IRenderPass* pass, const RenderPassEnvironmentResourceName& resourceName, RenderPassEnvironmentResourceType resourceType);
		void addRenderPassWriteResource(const IRenderPass* pass, const RenderPassEnvironmentResourceName& resourceName, RenderPassEnvironmentResourceType resourceType);

		bool build(RenderPassGraph& graph) const;
	};
}
