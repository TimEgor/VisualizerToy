#include "RenderPassGraphBuilder.h"

#include <unordered_set>
#include <cassert>

using RenderGraphResourcesHashesContainer = std::unordered_set<VT::RenderPassGraph::LayerResoureInfoHashType>;
using RenderGraphLayerResourceHashContainer = std::vector<RenderGraphResourcesHashesContainer>;

void VT::RenderPassGraphBuilder::BuildingContext::clear()
{
	m_passesDependencies.clear();
	m_passDepth.clear();
}

bool VT::RenderPassGraphBuilder::isPassDependent(size_t checkingPassIndex, size_t parentCheckingPassIndex) const
{
	if (checkingPassIndex == parentCheckingPassIndex)
	{
		return false;
	}

	const RenderPassResourceDependencyContainer& writingResources = m_writingResources[parentCheckingPassIndex];
	const RenderPassResourceDependencyContainer& readingResources = m_readingResources[checkingPassIndex];

	if (writingResources.empty() || readingResources.empty())
	{
		return false;
	}

	for (const RenderPassGraph::LayerResoureInfo& writingResource : writingResources)
	{
		for (const RenderPassGraph::LayerResoureInfo& readingResource : readingResources)
		{
			if (readingResource.m_hash == writingResource.m_hash)
			{
				return true;
			}
		}
	}

	return false;
}

void VT::RenderPassGraphBuilder::prepareDependentPasses() const
{
	const size_t passesCount = m_passes.size();
	m_buildContext.m_passesDependencies.resize(passesCount);

	for (size_t passIndex = 0; passIndex < passesCount; ++passIndex)
	{
		const RenderPassResourceDependencyContainer& writingResources = m_writingResources[passIndex];

		if (writingResources.empty())
		{
			continue;
		}

		PassIndexContainer& passDependency = m_buildContext.m_passesDependencies[passIndex];

		for (size_t dependentPassIndex = 0; dependentPassIndex < passesCount; ++dependentPassIndex)
		{
			if (isPassDependent(dependentPassIndex, passIndex))
			{
				passDependency.push_back(dependentPassIndex);
			}
		}
	}
}

size_t VT::RenderPassGraphBuilder::preparePassLayerDepth() const
{
	const size_t passesCount = m_passes.size();
	m_buildContext.m_passDepth.resize(passesCount, 0);

	size_t maxDepth = 0;

	for (size_t passIndex = 0; passIndex < passesCount; ++passIndex)
	{
		const PassIndexContainer& passDependencies = m_buildContext.m_passesDependencies[passIndex];
		const size_t dependenciesCount = passDependencies.size();
		for (size_t dependentIndex = 0; dependentIndex < dependenciesCount; ++dependentIndex)
		{
			const size_t dependentPassIndex = passDependencies[dependentIndex];
			const size_t nextDepthVal = m_buildContext.m_passDepth[passIndex] + 1;

			if (m_buildContext.m_passDepth[dependentPassIndex] < nextDepthVal)
			{
				m_buildContext.m_passDepth[dependentPassIndex] = nextDepthVal;
				maxDepth = std::max(maxDepth, nextDepthVal);
			}
		}
	}

	return maxDepth;
}

VT::GraphPassID VT::RenderPassGraphBuilder::getNextPassID()
{
	return m_passID++;
}

VT::GraphPassID VT::RenderPassGraphBuilder::addRenderPass(IRenderPass* pass)
{
	assert(pass);

	GraphPassID passID = getNextPassID();

	m_passes.push_back(RenderPassGraph::RenderPass{ pass, passID });
	m_passesIndices.insert(std::make_pair(pass, m_passes.size() - 1));

	m_readingResources.emplace_back();
	m_writingResources.emplace_back();

	return passID;
}

void VT::RenderPassGraphBuilder::addRenderPassReadResource(const IRenderPass* pass,
	const RenderPassEnvironmentResourceName& resourceName, RenderPassEnvironmentResourceType resourceType)
{
	assert(pass);
	const size_t passIndex = m_passesIndices.at(const_cast<IRenderPass*>(pass));
	m_readingResources[passIndex].emplace_back(resourceName.hash(), resourceType);
}

void VT::RenderPassGraphBuilder::addRenderPassWriteResource(const IRenderPass* pass,
	const RenderPassEnvironmentResourceName& resourceName, RenderPassEnvironmentResourceType resourceType)
{
	assert(pass);
	const size_t passIndex = m_passesIndices.at(const_cast<IRenderPass*>(pass));
	m_writingResources[passIndex].emplace_back(resourceName.hash(), resourceType);
}

bool VT::RenderPassGraphBuilder::build(RenderPassGraph& graph) const
{
	m_buildContext.clear();

	prepareDependentPasses();
	const size_t maxDepth = preparePassLayerDepth();
	const size_t layersCount = maxDepth + 1;

	graph.clear();
	graph.m_layers.resize(layersCount);

	RenderGraphLayerResourceHashContainer writingLayerResources(layersCount);
	RenderGraphLayerResourceHashContainer readingLayerResources(layersCount);

	const size_t passesCount = m_passes.size();
	for (size_t passIndex = 0; passIndex < passesCount; ++passIndex)
	{
		const size_t depth = m_buildContext.m_passDepth[passIndex];
		graph.m_layers[depth].m_passes.push_back(m_passes[passIndex]);

		RenderGraphResourcesHashesContainer& readingLayerResourceHashes = readingLayerResources[depth];
		const RenderPassResourceDependencyContainer& readingLayerResources = m_readingResources[passIndex];

		for (const RenderPassGraph::LayerResoureInfo& resourceInfo : readingLayerResources)
		{
			auto findResIter = readingLayerResourceHashes.find(resourceInfo.m_hash);
			if (findResIter == readingLayerResourceHashes.end())
			{
				readingLayerResourceHashes.insert(resourceInfo.m_hash);
				graph.m_layers[depth].m_readingResoures.push_back(resourceInfo);
			}
		}

		RenderGraphResourcesHashesContainer& writingLayerResourceHashes = writingLayerResources[depth];
		const RenderPassResourceDependencyContainer& writingPassResources = m_writingResources[passIndex];

		for (const RenderPassGraph::LayerResoureInfo& resourceInfo : writingPassResources)
		{
			auto findResIter = writingLayerResourceHashes.find(resourceInfo.m_hash);
			if (findResIter == writingLayerResourceHashes.end())
			{
				writingLayerResourceHashes.insert(resourceInfo.m_hash);
				graph.m_layers[depth].m_writingResoures.push_back(resourceInfo);
			}
		}
	}

	return true;
}
