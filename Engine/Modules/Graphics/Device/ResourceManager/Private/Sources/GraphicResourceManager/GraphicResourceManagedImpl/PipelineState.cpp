#include "Common.h"
#include "GraphicPipeline/IPipelineBindingLayout.h"

void VT::GraphicResourceManager::deletePipelineStateInternal(IPipelineState* state)
{
	getGraphicDevice()->destroyPipelineState(state);
}

VT::PipelineStateReference VT::GraphicResourceManager::getPipelineState(const PipelineStateInfo& desc,
	const IPipelineBindingLayout* bindingLayout, InputLayoutConstReference inputLayout)
{
	assert(bindingLayout);

	PipelineStateHash id = desc.getHash();

	hashCombine(id, bindingLayout->getHash());

	if (inputLayout)
	{
		hashCombine(id, inputLayout->getDesc().getHash());
	}

	const PipelineStateCollection::PipelineStateAccessInfo accessInfo = m_pipelineStateCollection.getOrAddPipelineState(id);
	if (accessInfo.m_isNew)
	{
		const InputLayoutDesc* inputlayoutDesc = inputLayout ? &inputLayout->getDesc() : nullptr;
		IPipelineState* state = getGraphicDevice()->createPipelineState(desc, bindingLayout, inputlayoutDesc);
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