#include "RenderPassGraph.h"

#include "Core/HashFunctions/StdHashCombine.h"

VT::RenderPassGraph::LayerResoureInfoHashType VT::RenderPassGraph::LayerResoureInfo::hash(const LayerResoureInfo& info)
{
	LayerResoureInfoHashType result = 0;

	hashCombine(result, info.m_id);
	hashCombine(result, info.m_type);

	return result;
}

void VT::RenderPassGraph::clear()
{
	m_layers = LayerDataContainer();
}