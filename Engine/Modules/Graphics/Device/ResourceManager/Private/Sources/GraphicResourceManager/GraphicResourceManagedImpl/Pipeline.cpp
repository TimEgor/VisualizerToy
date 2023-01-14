#include "Common.h"
#include "GraphicPipeline/IPipelineBindingLayout.h"

void VT::GraphicResourceManager::deletePipelineStateInternal(IPipelineState* state)
{
	getGraphicDevice()->destroyPipelineState(state);
}

VT::PipelineStateReference VT::GraphicResourceManager::getPipelineState(const PipelineStateInfo& desc,
	PipelineBindingLayoutConstReference bindingLayout, InputLayoutConstReference inputLayout)
{
	PipelineStateHash id = desc.getHash();

	IPipelineBindingLayout* nativeBindingLayout = nullptr;
	if (bindingLayout && bindingLayout->getResource())
	{
		nativeBindingLayout = bindingLayout->getResource();
		hashCombine(id, nativeBindingLayout->getHash());
	}

	if (inputLayout)
	{
		hashCombine(id, inputLayout->getDesc().getHash());
	}

	const PipelineStateCollection::AccessInfo accessInfo = m_pipelineStateCollection.getOrAddHandle(id);
	if (accessInfo.m_isNew)
	{
		const InputLayoutDesc* inputlayoutDesc = inputLayout ? &inputLayout->getDesc() : nullptr;
		IPipelineState* state = getGraphicDevice()->createPipelineState(desc, nativeBindingLayout, inputlayoutDesc);
		accessInfo.m_handle.getObjectCast<ManagedPipelineStateResourceHandle>()->setResource(state);
	}

	return accessInfo.m_handle;
}

//

void VT::GraphicResourceManager::deletePipelineBindingLayoutInternal(IPipelineBindingLayout* bindingLayout)
{
	getGraphicDevice()->destroyPipelineBindingLayout(bindingLayout);
}

VT::PipelineBindingLayoutReference VT::GraphicResourceManager::getPipelineBindingLayout(const PipelineBindingLayoutDesc& desc)
{
	PipelineBindingLayoutHash id = desc.getHash();

	const PipelineBindingLayoutCollection::AccessInfo accessInfo = m_pipelineBindingLayoutCollection.getOrAddHandle(id);
	if (accessInfo.m_isNew)
	{
		IPipelineBindingLayout* state = getGraphicDevice()->createPipelineBindingLayout(desc);
		accessInfo.m_handle.getObjectCast<ManagedPipelineBindingLayoutResourceHandle>()->setResource(state);
	}

	return accessInfo.m_handle;
}

//

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