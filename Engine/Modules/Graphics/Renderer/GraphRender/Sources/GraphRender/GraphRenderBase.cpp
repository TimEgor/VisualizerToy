#include "GraphRenderBase.h"

#include "Core/UtilitiesMacros.h"

#include "IRenderPass.h"
#include "RenderPassGraphBuilder.h"

bool VT::GraphRenderBase::initRenderGraph(const RenderPassGraphBuilder& builder)
{
	return builder.build(m_renderGraph);
}

void VT::GraphRenderBase::clearRenderGraph()
{
	m_renderGraph.clear();
	m_passDatas = RenderPassDataContainer();
}

bool VT::GraphRenderBase::initEnvironments(uint32_t count)
{
	assert(count);

	m_environments = new RenderPassEnvironment[count];
	VT_CHECK_RETURN_FALSE(m_environments);

	m_environmentIndex = 0;
	m_environmentsCount = count;

	return m_environments;
}

void VT::GraphRenderBase::releaseEnvironments()
{
	VT_SAFE_DESTROY_ARRAY(m_environments);
}

VT::RenderPassEnvironment& VT::GraphRenderBase::getEnvironment(uint32_t index)
{
	assert(index < m_environmentsCount);
	return m_environments[index];
}

void VT::GraphRenderBase::addRenderPassData(GraphPassID passID, IRenderPassData* data)
{
	m_passDatas[passID] = data;
}

void VT::GraphRenderBase::removeRenderPassData(GraphPassID passID)
{
	m_passDatas.erase(passID);
}

VT::IRenderPassData* VT::GraphRenderBase::getRenderPassData(GraphPassID passID)
{
	return m_passDatas.at(passID);
}

void VT::GraphRenderBase::draw(RenderDrawingContext& context)
{
	assert(m_environments);
	assert(m_environmentsCount);

	const RenderPassEnvironment& currentEnvironment = m_environments[m_environmentIndex];

	RenderPassGraph::LayerDataContainer& graphLayers = m_renderGraph.getLayerData();
	for (RenderPassGraph::LayerData& layer : graphLayers)
	{
		RenderPassGraph::LayerRenderPassContainer& passes = layer.m_passes;
		for(RenderPassGraph::RenderPass& pass : passes)
		{
			const IRenderPassData* passData = m_passDatas[pass.m_id];
			pass.m_pass->execute(context, currentEnvironment, passData);
		}
	}

	++m_environmentIndex;
	if (m_environmentIndex >= m_environmentsCount)
	{
		m_environmentIndex = 0;
	}
}
