#include "Common.h"
#include "GraphicPipeline/IPipelineBindingLayout.h"

void VT::GraphicResourceManager::deleteGraphicPipelineStateInternal(IGraphicPipelineState* state)
{
	getGraphicDevice()->destroyGraphicPipelineState(state);
}

void VT::GraphicResourceManager::deleteComputePipelineStateInternal(IComputePipelineState* state)
{
	getGraphicDevice()->destroyComputePipelineState(state);
}

VT::GraphicPipelineStateReference VT::GraphicResourceManager::getGraphicPipelineState(const GraphicPipelineStateInfo& desc,
	PipelineBindingLayoutConstReference bindingLayout, InputLayoutConstReference inputLayout)
{
	PipelineStateHash id = desc.getHash();

	IPipelineBindingLayout* nativeBindingLayout = nullptr;
	if (bindingLayout && bindingLayout->getTypedObject())
	{
		nativeBindingLayout = bindingLayout->getTypedObject();
		hashCombine(id, nativeBindingLayout->getHash());
	}

	if (inputLayout)
	{
		hashCombine(id, inputLayout->getDesc().getHash());
	}

	const GraphicPipelineStateCollection::AccessInfo accessInfo = m_graphicPipelineStateCollection.getOrAddHandle(id);
	if (accessInfo.m_isNew)
	{
		const InputLayoutDesc* inputlayoutDesc = inputLayout ? &inputLayout->getDesc() : nullptr;
		IGraphicPipelineState* state = getGraphicDevice()->createGraphicPipelineState(desc, nativeBindingLayout, inputlayoutDesc);
		accessInfo.m_handle.getObjectCast<ManagedGraphicPipelineStateGraphicObjectHandle>()->setHandleObject(state);
	}

	return accessInfo.m_handle;
}

VT::ComputePipelineStateReference VT::GraphicResourceManager::getComputePipelineState(
	const ComputePipelineStateInfo& desc, PipelineBindingLayoutConstReference bindingLayout)
{
	PipelineStateHash id = desc.getHash();

	IPipelineBindingLayout* nativeBindingLayout = nullptr;
	if (bindingLayout && bindingLayout->getTypedObject())
	{
		nativeBindingLayout = bindingLayout->getTypedObject();
		hashCombine(id, nativeBindingLayout->getHash());
	}

	const ComputePipelineStateCollection::AccessInfo accessInfo = m_computePipelineStateCollection.getOrAddHandle(id);
	if (accessInfo.m_isNew)
	{
		IComputePipelineState* state = getGraphicDevice()->createComputePipelineState(desc, nativeBindingLayout);
		accessInfo.m_handle.getObjectCast<ManagedComputePipelineStateGraphicObjectHandle>()->setHandleObject(state);
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
		accessInfo.m_handle.getObjectCast<ManagedPipelineBindingLayoutGraphicObjectHandle>()->setHandleObject(state);
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