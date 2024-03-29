#include "ManagedGraphicDevice.h"

#include "Core/UtilitiesMacros.h"

bool VT::ManagedGraphicDevice::ManagedGraphicDevice::init(bool isSwapChainEnabled)
{
	VT_CHECK_INITIALIZATION(initDevice(isSwapChainEnabled));

	m_bufferStorage = createBufferStorage();
	VT_CHECK_INITIALIZATION(m_bufferStorage && m_bufferStorage->init(256, 1, 128));

	m_texture2DStorage = createTexture2DStorage();
	VT_CHECK_INITIALIZATION(m_texture2DStorage && m_texture2DStorage->init(256, 1, 64));

	m_vertexShaderStorage = createVertexShaderStorage();
	VT_CHECK_INITIALIZATION(m_vertexShaderStorage && m_vertexShaderStorage->init(64, 1, 16));

	m_pixelShaderStorage = createPixelShaderStorage();
	VT_CHECK_INITIALIZATION(m_pixelShaderStorage && m_pixelShaderStorage->init(128, 1, 16));

	m_computeShaderStorage = createComputeShaderStorage();
	VT_CHECK_INITIALIZATION(m_computeShaderStorage && m_computeShaderStorage->init(64, 1, 16));

	m_graphicPipelineStateStorage = createGraphicPipelineStateStorage();
	VT_CHECK_INITIALIZATION(m_graphicPipelineStateStorage && m_graphicPipelineStateStorage->init(64, 1, 32));

	m_computePipelineStateStorage = createComputePipelineStateStorage();
	VT_CHECK_INITIALIZATION(m_computePipelineStateStorage && m_computePipelineStateStorage->init(64, 1, 32));

	m_pipelineBindingLayoutStorage = createPipelineBindingLayoutStorage();
	VT_CHECK_INITIALIZATION(m_pipelineBindingLayoutStorage && m_pipelineBindingLayoutStorage->init(64, 0, 32));

	m_commandListStorage = createCommandListStorage();
	VT_CHECK_INITIALIZATION(m_commandListStorage && m_commandListStorage->init(32, 0, 16));

	m_fenceStorage = createFenceStorage();
	VT_CHECK_INITIALIZATION(m_fenceStorage && m_fenceStorage->init(64, 0, 32));

	m_descriptorStorage = createGraphicResourceDescriptorStorage();
	VT_CHECK_INITIALIZATION(m_descriptorStorage && m_descriptorStorage->init(256, 1, 128));

	return true;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::release()
{
	VT_SAFE_DESTROY_WITH_RELEASING(m_bufferStorage);

	VT_SAFE_DESTROY_WITH_RELEASING(m_texture2DStorage);

	VT_SAFE_DESTROY_WITH_RELEASING(m_vertexShaderStorage);
	VT_SAFE_DESTROY_WITH_RELEASING(m_pixelShaderStorage);
	VT_SAFE_DESTROY_WITH_RELEASING(m_computeShaderStorage);

	VT_SAFE_DESTROY_WITH_RELEASING(m_graphicPipelineStateStorage);
	VT_SAFE_DESTROY_WITH_RELEASING(m_computePipelineStateStorage);
	VT_SAFE_DESTROY_WITH_RELEASING(m_pipelineBindingLayoutStorage);

	VT_SAFE_DESTROY_WITH_RELEASING(m_commandListStorage);

	VT_SAFE_DESTROY_WITH_RELEASING(m_fenceStorage);

	VT_SAFE_DESTROY_WITH_RELEASING(m_descriptorStorage);

	releaseDevice();
}

VT::IGraphicResourceDescriptor* VT::ManagedGraphicDevice::ManagedGraphicDevice::createShaderResourceDescriptor(
	IGraphicResource* resource)
{
	assert(m_descriptorStorage);

	ManagedGraphicResourceDescriptorStorageInfoBase::NewObjectInfo newObjectInfo = m_descriptorStorage->addNewObject();
	if (!createShaderResourceDescriptor(newObjectInfo.m_objectPtr, resource))
	{
		m_descriptorStorage->removeObject(newObjectInfo.m_objectHandle);
		return nullptr;
	}

	newObjectInfo.m_objectPtr->m_handle = newObjectInfo.m_objectHandle;

	return newObjectInfo.m_objectPtr;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::destroyShaderResourceDescriptor(
	IGraphicResourceDescriptor* descriptor)
{
	assert(descriptor);
	assert(m_descriptorStorage);

	ManagedGraphicResourceDescriptorBase* managedDescriptor = reinterpret_cast<ManagedGraphicResourceDescriptorBase*>(descriptor);

	destroyRenderTargetDescriptor(managedDescriptor);
	m_descriptorStorage->removeObject(managedDescriptor->getHandle());
}

VT::IGraphicResourceDescriptor* VT::ManagedGraphicDevice::ManagedGraphicDevice::createUnorderedAccessResourceDescriptor(
	IGraphicResource* resource)
{
	assert(m_descriptorStorage);

	ManagedGraphicResourceDescriptorStorageInfoBase::NewObjectInfo newObjectInfo = m_descriptorStorage->addNewObject();
	if (!createUnorderedAccessResourceDescriptor(newObjectInfo.m_objectPtr, resource))
	{
		m_descriptorStorage->removeObject(newObjectInfo.m_objectHandle);
		return nullptr;
	}

	newObjectInfo.m_objectPtr->m_handle = newObjectInfo.m_objectHandle;

	return newObjectInfo.m_objectPtr;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::destroyUnorderedAccessResourceDescriptor(
	IGraphicResourceDescriptor* descriptor)
{
	assert(descriptor);
	assert(m_descriptorStorage);

	ManagedGraphicResourceDescriptorBase* managedDescriptor = reinterpret_cast<ManagedGraphicResourceDescriptorBase*>(descriptor);

	destroyRenderTargetDescriptor(managedDescriptor);
	m_descriptorStorage->removeObject(managedDescriptor->getHandle());
}

VT::IGPUBuffer* VT::ManagedGraphicDevice::ManagedGraphicDevice::createBuffer(const GPUBufferDesc& desc,
                                                                             GraphicResourceStateValueType initialState, const InitialGPUBufferData* initialData)
{
	assert(m_bufferStorage);

	ManagedGPUBufferStorageInfoBase::NewObjectInfo newObjectInfo = m_bufferStorage->addNewObject();
	if (!createBuffer(newObjectInfo.m_objectPtr, desc, initialState, initialData))
	{
		m_bufferStorage->removeObject(newObjectInfo.m_objectHandle);
		return nullptr;
	}

	newObjectInfo.m_objectPtr->m_handle = newObjectInfo.m_objectHandle;

	return newObjectInfo.m_objectPtr;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::destroyBuffer(IGPUBuffer* buffer)
{
	assert(buffer);
	assert(m_bufferStorage);

	ManagedGPUBufferBase* managedBuffer = reinterpret_cast<ManagedGPUBufferBase*>(buffer);

	destroyBuffer(managedBuffer);
	m_bufferStorage->removeObject(managedBuffer->getHandle());
}

VT::IGraphicResourceDescriptor* VT::ManagedGraphicDevice::ManagedGraphicDevice::createBufferResourceDescriptor(IGPUBuffer* buffer)
{
	assert(m_descriptorStorage);

	ManagedGraphicResourceDescriptorStorageInfoBase::NewObjectInfo newObjectInfo = m_descriptorStorage->addNewObject();
	if (!createBufferResourceDescriptor(newObjectInfo.m_objectPtr, buffer))
	{
		m_descriptorStorage->removeObject(newObjectInfo.m_objectHandle);
		return nullptr;
	}

	newObjectInfo.m_objectPtr->m_handle = newObjectInfo.m_objectHandle;

	return newObjectInfo.m_objectPtr;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::destroyBufferResourceDescriptor(IGraphicResourceDescriptor* descriptor)
{
	assert(descriptor);
	assert(m_descriptorStorage);

	ManagedGraphicResourceDescriptorBase* managedDescriptor = reinterpret_cast<ManagedGraphicResourceDescriptorBase*>(descriptor);

	destroyRenderTargetDescriptor(managedDescriptor);
	m_descriptorStorage->removeObject(managedDescriptor->getHandle());
}

VT::ITexture2D* VT::ManagedGraphicDevice::ManagedGraphicDevice::createTexture2D(const Texture2DDesc& desc, GraphicResourceStateValueType initialState)
{
	assert(m_descriptorStorage);

	ManagedTexture2DStorageInfoBase::NewObjectInfo newObjectInfo = m_texture2DStorage->addNewObject();
	if (!createTexture2D(newObjectInfo.m_objectPtr, desc, initialState))
	{
		m_texture2DStorage->removeObject(newObjectInfo.m_objectHandle);
		return nullptr;
	}

	newObjectInfo.m_objectPtr->m_handle = newObjectInfo.m_objectHandle;

	return newObjectInfo.m_objectPtr;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::destroyTexture2D(ITexture2D* texture)
{
	assert(texture);
	assert(m_texture2DStorage);

	ManagedTexture2DBase* managedTexture = reinterpret_cast<ManagedTexture2DBase*>(texture);

	destroyTexture2D(managedTexture);
	m_texture2DStorage->removeObject(managedTexture->getHandle());
}

VT::IGraphicResourceDescriptor* VT::ManagedGraphicDevice::ManagedGraphicDevice::createRenderTargetDescriptor(ITexture* texture)
{
	assert(m_descriptorStorage);

	ManagedGraphicResourceDescriptorStorageInfoBase::NewObjectInfo newObjectInfo = m_descriptorStorage->addNewObject();
	if (!createRenderTargetDescriptor(newObjectInfo.m_objectPtr, texture))
	{
		m_descriptorStorage->removeObject(newObjectInfo.m_objectHandle);
		return nullptr;
	}

	newObjectInfo.m_objectPtr->m_handle = newObjectInfo.m_objectHandle;

	return newObjectInfo.m_objectPtr;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::destroyRenderTargetDescriptor(IGraphicResourceDescriptor* descriptor)
{
	assert(descriptor);
	assert(m_descriptorStorage);

	ManagedGraphicResourceDescriptorBase* managedDescriptor = reinterpret_cast<ManagedGraphicResourceDescriptorBase*>(descriptor);

	destroyRenderTargetDescriptor(managedDescriptor);
	m_descriptorStorage->removeObject(managedDescriptor->getHandle());
}

VT::IGraphicResourceDescriptor* VT::ManagedGraphicDevice::ManagedGraphicDevice::createDepthStencilDescriptor(ITexture* texture)
{
	assert(m_descriptorStorage);

	ManagedGraphicResourceDescriptorStorageInfoBase::NewObjectInfo newObjectInfo = m_descriptorStorage->addNewObject();
	if (!createDepthStencilDescriptor(newObjectInfo.m_objectPtr, texture))
	{
		m_descriptorStorage->removeObject(newObjectInfo.m_objectHandle);
		return nullptr;
	}

	newObjectInfo.m_objectPtr->m_handle = newObjectInfo.m_objectHandle;

	return newObjectInfo.m_objectPtr;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::destroyDepthStencilDescriptor(IGraphicResourceDescriptor* descriptor)
{
	assert(descriptor);
	assert(m_descriptorStorage);

	ManagedGraphicResourceDescriptorBase* managedDescriptor = reinterpret_cast<ManagedGraphicResourceDescriptorBase*>(descriptor);

	destroyRenderTargetDescriptor(managedDescriptor);
	m_descriptorStorage->removeObject(managedDescriptor->getHandle());
}

VT::IVertexShader* VT::ManagedGraphicDevice::ManagedGraphicDevice::createVertexShader(const void* code, size_t codeSize)
{
	assert(m_vertexShaderStorage);

	ManagedVertexShaderStorageInfoBase::NewObjectInfo newObjectInfo = m_vertexShaderStorage->addNewObject();
	if (!createVertexShader(newObjectInfo.m_objectPtr, code, codeSize))
	{
		m_vertexShaderStorage->removeObject(newObjectInfo.m_objectHandle);
		return nullptr;
	}

	newObjectInfo.m_objectPtr->m_handle = newObjectInfo.m_objectHandle;

	return newObjectInfo.m_objectPtr;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::destroyVertexShader(IVertexShader* shader)
{
	assert(shader);
	assert(m_vertexShaderStorage);

	ManagedVertexShaderBase* managedShader = reinterpret_cast<ManagedVertexShaderBase*>(shader);

	destroyVertexShader(managedShader);
	m_vertexShaderStorage->removeObject(managedShader->getHandle());
}

VT::IPixelShader* VT::ManagedGraphicDevice::ManagedGraphicDevice::createPixelShader(const void* code, size_t codeSize)
{
	assert(m_pixelShaderStorage);

	ManagedPixelShaderStorageInfoBase::NewObjectInfo newObjectInfo = m_pixelShaderStorage->addNewObject();
	if (!createPixelShader(newObjectInfo.m_objectPtr, code, codeSize))
	{
		m_pixelShaderStorage->removeObject(newObjectInfo.m_objectHandle);
		return nullptr;
	}

	newObjectInfo.m_objectPtr->m_handle = newObjectInfo.m_objectHandle;

	return newObjectInfo.m_objectPtr;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::destroyPixelShader(IPixelShader* shader)
{
	assert(shader);
	assert(m_pixelShaderStorage);

	ManagedPixelShaderBase* managedShader = reinterpret_cast<ManagedPixelShaderBase*>(shader);

	destroyPixelShader(managedShader);
	m_pixelShaderStorage->removeObject(managedShader->getHandle());
}

VT::IComputeShader* VT::ManagedGraphicDevice::ManagedGraphicDevice::createComputeShader(const void* code, size_t codeSize)
{
	assert(m_computeShaderStorage);

	ManagedComputeShaderStorageInfoBase::NewObjectInfo newObjectInfo = m_computeShaderStorage->addNewObject();
	if (!createComputeShader(newObjectInfo.m_objectPtr, code, codeSize))
	{
		m_computeShaderStorage->removeObject(newObjectInfo.m_objectHandle);
		return nullptr;
	}

	newObjectInfo.m_objectPtr->m_handle = newObjectInfo.m_objectHandle;

	return newObjectInfo.m_objectPtr;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::destroyComputeShader(IComputeShader* shader)
{
	assert(shader);
	assert(m_computeShaderStorage);

	ManagedComputeShaderBase* managedShader = reinterpret_cast<ManagedComputeShaderBase*>(shader);

	destroyComputeShader(managedShader);
	m_computeShaderStorage->removeObject(managedShader->getHandle());
}

VT::IGraphicPipelineState* VT::ManagedGraphicDevice::ManagedGraphicDevice::createGraphicPipelineState(
	const GraphicPipelineStateInfo& pipelineStateInfo, const IPipelineBindingLayout* bindingLayout, const InputLayoutDesc* inputLayoutDesc)
{
	assert(m_graphicPipelineStateStorage);

	ManagedGraphicPipelineStateStorageInfoBase::NewObjectInfo newObjectInfo = m_graphicPipelineStateStorage->addNewObject();
	if (!createGraphicPipelineState(newObjectInfo.m_objectPtr, pipelineStateInfo, bindingLayout, inputLayoutDesc))
	{
		m_graphicPipelineStateStorage->removeObject(newObjectInfo.m_objectHandle);
		return nullptr;
	}

	newObjectInfo.m_objectPtr->m_handle = newObjectInfo.m_objectHandle;

	return newObjectInfo.m_objectPtr;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::destroyGraphicPipelineState(IGraphicPipelineState* state)
{
	assert(state);
	assert(m_graphicPipelineStateStorage);

	ManagedGraphicPipelineStateBase* managedState = reinterpret_cast<ManagedGraphicPipelineStateBase*>(state);

	destroyGraphicPipelineState(managedState);
	m_graphicPipelineStateStorage->removeObject(managedState->getHandle());
}

VT::IComputePipelineState* VT::ManagedGraphicDevice::ManagedGraphicDevice::createComputePipelineState(
	const ComputePipelineStateInfo& info, const IPipelineBindingLayout* bindingLayout)
{
	assert(m_computePipelineStateStorage);

	ManagedComputePipelineStateStorageInfoBase::NewObjectInfo newObjectInfo = m_computePipelineStateStorage->addNewObject();
	if (!createComputePipelineState(newObjectInfo.m_objectPtr, info, bindingLayout))
	{
		m_computePipelineStateStorage->removeObject(newObjectInfo.m_objectHandle);
		return nullptr;
	}

	newObjectInfo.m_objectPtr->m_handle = newObjectInfo.m_objectHandle;

	return newObjectInfo.m_objectPtr;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::destroyComputePipelineState(IComputePipelineState* state)
{
	assert(state);
	assert(m_computePipelineStateStorage);

	ManagedComputePipelineStateBase* managedState = reinterpret_cast<ManagedComputePipelineStateBase*>(state);

	destroyComputePipelineState(managedState);
	m_computePipelineStateStorage->removeObject(managedState->getHandle());
}

VT::IPipelineBindingLayout* VT::ManagedGraphicDevice::ManagedGraphicDevice::createPipelineBindingLayout(const PipelineBindingLayoutDesc& desc)
{
	assert(m_pipelineBindingLayoutStorage);

	ManagedPipelineBindingLayoutStorageInfoBase::NewObjectInfo newObjectInfo = m_pipelineBindingLayoutStorage->addNewObject();
	if (!createPipelineBindingLayout(newObjectInfo.m_objectPtr, desc))
	{
		m_pipelineBindingLayoutStorage->removeObject(newObjectInfo.m_objectHandle);
		return nullptr;
	}

	newObjectInfo.m_objectPtr->m_handle = newObjectInfo.m_objectHandle;

	return newObjectInfo.m_objectPtr;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::destroyPipelineBindingLayout(IPipelineBindingLayout* layout)
{
	assert(layout);
	assert(m_pipelineBindingLayoutStorage);

	ManagedPipelineBindingLayoutBase* managedLayout = reinterpret_cast<ManagedPipelineBindingLayoutBase*>(layout);

	destroyPipelineBindingLayout(managedLayout);
	m_pipelineBindingLayoutStorage->removeObject(managedLayout->getHandle());
}

VT::ICommandList* VT::ManagedGraphicDevice::ManagedGraphicDevice::createCommandList()
{
	assert(m_commandListStorage);

	ManagedCommandListStorageInfoBase::NewObjectInfo newObjectInfo = m_commandListStorage->addNewObject();
	if (!createCommandList(newObjectInfo.m_objectPtr))
	{
		m_commandListStorage->removeObject(newObjectInfo.m_objectHandle);
		return nullptr;
	}

	newObjectInfo.m_objectPtr->m_handle = newObjectInfo.m_objectHandle;

	return newObjectInfo.m_objectPtr;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::destroyCommandList(ICommandList* list)
{
	assert(list);
	assert(m_commandListStorage);

	ManagedCommandListBase* managedPool = reinterpret_cast<ManagedCommandListBase*>(list);

	destroyCommandList(managedPool);
	m_commandListStorage->removeObject(managedPool->getHandle());
}

VT::IFence* VT::ManagedGraphicDevice::ManagedGraphicDevice::createFence()
{
	assert(m_fenceStorage);

	ManagedFenceStorageInfoBase::NewObjectInfo newObjectInfo = m_fenceStorage->addNewObject();
	if (!createFence(newObjectInfo.m_objectPtr))
	{
		m_fenceStorage->removeObject(newObjectInfo.m_objectHandle);
		return nullptr;
	}

	newObjectInfo.m_objectPtr->m_handle = newObjectInfo.m_objectHandle;

	return newObjectInfo.m_objectPtr;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::destroyFence(IFence* fence)
{
	assert(fence);
	assert(m_fenceStorage);

	ManagedFenceBase* managedFence = reinterpret_cast<ManagedFenceBase*>(fence);

	destroyFence(managedFence);
	m_fenceStorage->removeObject(managedFence->getHandle());
}