#include "Common.h"

#include "GraphicPipeline/IRenderPass.h"

void VT::GraphicResourceManager::deletePipelineStateInternal(IPipelineState* state)
{
	getGraphicDevice()->destroyPipelineState(state);
}

VT::PipelineStateReference VT::GraphicResourceManager::getPipelineState(const PipelineStateInfo& desc, const IRenderPass& renderPass)
{
	PipelineStateHash id = 0;

	hashCombine(id, desc.getHash());
	hashCombine(id, renderPass.getHash());

	const PipelineStateCollection::PipelineStateAccessInfo accessInfo = m_pipelineStateCollection.getOrAddPipelineState(id);
	if (accessInfo.m_isNew)
	{
		IPipelineState* state = getGraphicDevice()->createPipelineState(desc, &renderPass);
		accessInfo.m_state.getObjectCast<ManagedPipelineStateResourceHandle>()->setResource(state);
	}

	return accessInfo.m_state;
}

bool VT::GraphicResourceManager::isValidPipelineState(PipelineStateHandleID handle) const
{
	return m_pipelineStateCollection.isValid(handle);
}