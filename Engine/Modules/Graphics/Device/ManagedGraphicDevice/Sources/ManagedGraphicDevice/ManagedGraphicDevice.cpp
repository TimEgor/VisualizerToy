#include "ManagedGraphicDevice.h"

#include "Core/UtilitiesMacros.h"

bool VT::ManagedGraphicDevice::ManagedGraphicDevice::init(bool isSwapChainEnabled)
{
	VT_CHECK_INITIALIZATION(initDevice(isSwapChainEnabled));

	//m_bufferStorage = createBufferStorage();
	//VT_CHECK_INITIALIZATION(m_bufferStorage && m_bufferStorage->init(256, 1, 128));

	m_texture2DStorage = createTexture2DStorage();
	VT_CHECK_INITIALIZATION(m_texture2DStorage && m_texture2DStorage->init(256, 1, 64));

	//m_vertexShaderStorage = createVertexShaderStorage();
	//VT_CHECK_INITIALIZATION(m_vertexShaderStorage && m_vertexShaderStorage->init(64, 1, 16));

	//m_pixelShaderStorage = createPixelShaderStorage();
	//VT_CHECK_INITIALIZATION(m_pixelShaderStorage && m_pixelShaderStorage->init(128, 1, 16));

	//m_pipelineStateStorage = createPipelineStateStorage();
	//VT_CHECK_INITIALIZATION(m_pipelineStateStorage && m_pipelineStateStorage->init(256, 1, 64));

	//m_pipelineBindingLayoutStorage = createPipelineBindingLayoutStorage();
	//VT_CHECK_INITIALIZATION(m_pipelineBindingLayoutStorage && m_pipelineBindingLayoutStorage->init(64, 0, 32));

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

	VT_SAFE_DESTROY_WITH_RELEASING(m_pipelineStateStorage);
	VT_SAFE_DESTROY_WITH_RELEASING(m_pipelineBindingLayoutStorage);

	VT_SAFE_DESTROY_WITH_RELEASING(m_commandListStorage);

	VT_SAFE_DESTROY_WITH_RELEASING(m_fenceStorage);

	VT_SAFE_DESTROY_WITH_RELEASING(m_descriptorStorage);

	releaseDevice();
}

VT::IGPUBuffer* VT::ManagedGraphicDevice::ManagedGraphicDevice::createBuffer(const GPUBufferDesc& desc)
{
	assert(m_bufferStorage);

	ManagedGPUBufferStorageInfoBase::NewObjectInfo newObjectInfo = m_bufferStorage->addNewObject();
	if (!createBuffer(newObjectInfo.m_objectPtr, desc))
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

VT::IPipelineState* VT::ManagedGraphicDevice::ManagedGraphicDevice::createPipelineState(const PipelineStateInfo& pipelineStateInfo,
	const IPipelineBindingLayout* bindingLayout, const InputLayoutDesc* inputLayoutDesc)
{
	assert(m_pipelineStateStorage);

	ManagedPipelineStateStorageInfoBase::NewObjectInfo newObjectInfo = m_pipelineStateStorage->addNewObject();
	if (!createPipelineState(newObjectInfo.m_objectPtr, pipelineStateInfo, bindingLayout, inputLayoutDesc))
	{
		m_pipelineStateStorage->removeObject(newObjectInfo.m_objectHandle);
		return nullptr;
	}

	newObjectInfo.m_objectPtr->m_handle = newObjectInfo.m_objectHandle;

	return newObjectInfo.m_objectPtr;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::destroyPipelineState(IPipelineState* state)
{
	assert(state);
	assert(m_pipelineStateStorage);

	ManagedPipelineStateBase* managedState = reinterpret_cast<ManagedPipelineStateBase*>(state);

	destroyPipelineState(managedState);
	m_pipelineStateStorage->removeObject(managedState->getHandle());
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