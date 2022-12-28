#include "Common.h"

void VT::GraphicResourceManager::deletePipelineStateInternal(IPipelineState* state)
{
	getGraphicDevice()->destroyPipelineState(state);
}

VT::PipelineStateReference VT::GraphicResourceManager::getPipelineState(const PipelineStateInfo& desc)
{
	PipelineStateHash id = desc.getHash();

	const PipelineStateCollection::PipelineStateAccessInfo accessInfo = m_pipelineStateCollection.getOrAddPipelineState(id);
	if (accessInfo.m_isNew)
	{
		IPipelineState* state = getGraphicDevice()->createPipelineState(desc);
		accessInfo.m_state.getObjectCast<ManagedPipelineStateResourceHandle>()->setResource(state);
	}

	return accessInfo.m_state;
}

bool VT::GraphicResourceManager::isValidPipelineState(PipelineStateHandleID handle) const
{
	return m_pipelineStateCollection.isValid(handle);
}