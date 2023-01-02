#include "Common.h"

void VT::GraphicResourceManager::deletePipelineStateInternal(IPipelineState* state)
{
	getGraphicDevice()->destroyPipelineState(state);
}

VT::PipelineStateReference VT::GraphicResourceManager::getPipelineState(const PipelineStateInfo& desc, InputLayoutConstReference inputLayout)
{
	PipelineStateHash id = desc.getHash();

	if (inputLayout)
	{
		hashCombine(id, inputLayout->getDesc().getHash());
	}

	const PipelineStateCollection::PipelineStateAccessInfo accessInfo = m_pipelineStateCollection.getOrAddPipelineState(id);
	if (accessInfo.m_isNew)
	{
		const InputLayoutDesc* inputlayoutDesc = inputLayout ? &inputLayout->getDesc() : nullptr;
		IPipelineState* state = getGraphicDevice()->createPipelineState(desc, inputlayoutDesc);
		accessInfo.m_state.getObjectCast<ManagedPipelineStateResourceHandle>()->setResource(state);
	}

	return accessInfo.m_state;
}

void VT::GraphicResourceManager::deleteInputLayoutReference(InputLayoutHandleID handleID)
{
	m_inputLayoutCollection.removeInputLayoutHandle(handleID);
}

VT::InputLayoutReference VT::GraphicResourceManager::addInputLayout(const InputLayoutDesc& desc)
{
	return m_inputLayoutCollection.addInputLayout(desc);
}

VT::InputLayoutReference VT::GraphicResourceManager::getInputLayout(InputLayoutHash hash)
{
	return m_inputLayoutCollection.getInputLayout(hash);
}