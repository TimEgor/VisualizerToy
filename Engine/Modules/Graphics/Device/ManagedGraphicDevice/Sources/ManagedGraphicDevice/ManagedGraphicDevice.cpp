#include "ManagedGraphicDevice.h"

#include "Core/UtilitiesMacros.h"

bool VT::ManagedGraphicDevice::ManagedGraphicDevice::init(bool isSwapChainEnabled)
{
	VT_CHECK_INITIALIZATION(initDevice(isSwapChainEnabled));

	if (isSwapChainEnabled)
	{
		m_swapChainStorage = createSwapChainStorage();
		VT_CHECK_INITIALIZATION(m_swapChainStorage && m_swapChainStorage->init(4, 0, 0));
	}

	m_bufferStorage = createBufferStorage();
	VT_CHECK_INITIALIZATION(m_bufferStorage && m_bufferStorage->init(256, 1, 128));

	m_texture2DStorage = createTexture2DStorage();
	VT_CHECK_INITIALIZATION(m_texture2DStorage && m_texture2DStorage->init(256, 1, 64));

	m_texture2DViewStorage = createTexture2DViewStorage();
	VT_CHECK_INITIALIZATION(m_texture2DViewStorage && m_texture2DViewStorage->init(256, 1, 64));

	m_vertexShaderStorage = createVertexShaderStorage();
	VT_CHECK_INITIALIZATION(m_vertexShaderStorage && m_vertexShaderStorage->init(64, 1, 16));

	m_pixelShaderStorage = createPixelShaderStorage();
	VT_CHECK_INITIALIZATION(m_pixelShaderStorage && m_pixelShaderStorage->init(128, 1, 16));

	m_pipelineStateStorage = createPipelineStateStorage();
	VT_CHECK_INITIALIZATION(m_pipelineStateStorage && m_pipelineStateStorage->init(256, 1, 64));

	m_commandPoolStorage = createCommandPoolStorage();
	VT_CHECK_INITIALIZATION(m_commandPoolStorage && m_commandPoolStorage->init(32, 0, 16));

	m_commandListStorage = createCommandListStorage();
	VT_CHECK_INITIALIZATION(m_commandListStorage && m_commandListStorage->init(32, 0, 16));

	m_fenceStorage = createFenceStorage();
	VT_CHECK_INITIALIZATION(m_fenceStorage && m_fenceStorage->init(64, 0, 32));

	m_semaphoreStorage = createSemaphoreStorage();
	VT_CHECK_INITIALIZATION(m_semaphoreStorage && m_semaphoreStorage->init(64, 0, 32));

	return true;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::release()
{

	VT_SAFE_DESTROY_WITH_RELEASING(m_swapChainStorage);

	VT_SAFE_DESTROY_WITH_RELEASING(m_bufferStorage);

	VT_SAFE_DESTROY_WITH_RELEASING(m_texture2DStorage);
	VT_SAFE_DESTROY_WITH_RELEASING(m_texture2DViewStorage);

	VT_SAFE_DESTROY_WITH_RELEASING(m_vertexShaderStorage);
	VT_SAFE_DESTROY_WITH_RELEASING(m_pixelShaderStorage);

	VT_SAFE_DESTROY_WITH_RELEASING(m_pipelineStateStorage);

	VT_SAFE_DESTROY_WITH_RELEASING(m_commandPoolStorage);
	VT_SAFE_DESTROY_WITH_RELEASING(m_commandListStorage);

	VT_SAFE_DESTROY_WITH_RELEASING(m_fenceStorage);
	VT_SAFE_DESTROY_WITH_RELEASING(m_semaphoreStorage);

	releaseDevice();
}

VT::ISwapChain* VT::ManagedGraphicDevice::ManagedGraphicDevice::createSwapChain(const SwapChainDesc& desc, const IWindow* window)
{
	assert(m_swapChainStorage);

	ManagedSwapChainStorageInfoBase::NewObjectInfo newObjectInfo = m_swapChainStorage->addNewObject();
	if (!createSwapChain(newObjectInfo.m_objectPtr, desc, window))
	{
		m_swapChainStorage->removeObject(newObjectInfo.m_objectHandle);
		return nullptr;
	}

	newObjectInfo.m_objectPtr->m_handle = newObjectInfo.m_objectHandle;

	return newObjectInfo.m_objectPtr;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::destroySwapChain(ISwapChain* swapChain)
{
	assert(swapChain);
	assert(m_swapChainStorage);

	ManagedSwapChainBase* managedSwapChain = reinterpret_cast<ManagedSwapChainBase*>(swapChain);

	destroySwapChain(managedSwapChain);
	m_swapChainStorage->removeObject(managedSwapChain->getHandle());
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

VT::ITexture2DView* VT::ManagedGraphicDevice::ManagedGraphicDevice::createTexture2DView(ITexture2D* texture, const TextureViewDesc& desc)
{
	assert(texture);

	ManagedTexture2DViewStorageInfoBase::NewObjectInfo newObjectInfo = m_texture2DViewStorage->addNewObject();
	if (!createTexture2DView(newObjectInfo.m_objectPtr, texture, desc))
	{
		m_texture2DViewStorage->removeObject(newObjectInfo.m_objectHandle);
		return nullptr;
	}

	newObjectInfo.m_objectPtr->m_handle = newObjectInfo.m_objectHandle;

	return newObjectInfo.m_objectPtr;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::destroyTexture2DView(ITexture2DView* view)
{
	assert(view);
	assert(m_texture2DViewStorage);

	ManagedTexture2DViewBase* managedTextureView = reinterpret_cast<ManagedTexture2DViewBase*>(view);

	destroyTexture2DView(managedTextureView);
	m_texture2DViewStorage->removeObject(managedTextureView->getHandle());
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

VT::IPipelineState* VT::ManagedGraphicDevice::ManagedGraphicDevice::createPipelineState(
	const PipelineStateInfo& pipelineStateInfo, const InputLayoutDesc* inputLayoutDesc)
{
	assert(m_pipelineStateStorage);

	ManagedPipelineStateStorageInfoBase::NewObjectInfo newObjectInfo = m_pipelineStateStorage->addNewObject();
	if (!createPipelineState(newObjectInfo.m_objectPtr, pipelineStateInfo, inputLayoutDesc))
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

VT::ICommandPool* VT::ManagedGraphicDevice::ManagedGraphicDevice::createCommandPool()
{
	assert(m_commandPoolStorage);

	ManagedCommandPoolStorageInfoBase::NewObjectInfo newObjectInfo = m_commandPoolStorage->addNewObject();
	if (!createCommandPool(newObjectInfo.m_objectPtr))
	{
		m_commandPoolStorage->removeObject(newObjectInfo.m_objectHandle);
		return nullptr;
	}

	newObjectInfo.m_objectPtr->m_handle = newObjectInfo.m_objectHandle;

	return newObjectInfo.m_objectPtr;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::destroyCommandPool(ICommandPool* commandPool)
{
	assert(commandPool);
	assert(m_commandPoolStorage);

	ManagedCommandPoolBase* managedPool = reinterpret_cast<ManagedCommandPoolBase*>(commandPool);

	destroyCommandPool(managedPool);
	m_commandPoolStorage->removeObject(managedPool->getHandle());
}

VT::ICommandList* VT::ManagedGraphicDevice::ManagedGraphicDevice::allocateCommandList(ICommandPool* pool)
{
	assert(m_commandListStorage);

	ManagedCommandListStorageInfoBase::NewObjectInfo newObjectInfo = m_commandListStorage->addNewObject();
	if (!allocateCommandList(newObjectInfo.m_objectPtr, pool))
	{
		m_commandListStorage->removeObject(newObjectInfo.m_objectHandle);
		return nullptr;
	}

	newObjectInfo.m_objectPtr->m_handle = newObjectInfo.m_objectHandle;

	return newObjectInfo.m_objectPtr;
}

VT::IFence* VT::ManagedGraphicDevice::ManagedGraphicDevice::createFence(bool signaled)
{
	assert(m_fenceStorage);

	ManagedFenceStorageInfoBase::NewObjectInfo newObjectInfo = m_fenceStorage->addNewObject();
	if (!createFence(newObjectInfo.m_objectPtr, signaled))
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

VT::ISemaphore* VT::ManagedGraphicDevice::ManagedGraphicDevice::createSemaphore()
{
	assert(m_semaphoreStorage);

	ManagedSemaphoreStorageInfoBase::NewObjectInfo newObjectInfo = m_semaphoreStorage->addNewObject();
	if (!createSemaphore(newObjectInfo.m_objectPtr))
	{
		m_fenceStorage->removeObject(newObjectInfo.m_objectHandle);
		return nullptr;
	}

	newObjectInfo.m_objectPtr->m_handle = newObjectInfo.m_objectHandle;

	return newObjectInfo.m_objectPtr;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::destroySemaphore(ISemaphore* semaphore)
{
	assert(semaphore);
	assert(m_semaphoreStorage);

	ManagedSemaphoreBase* managedSemaphore = reinterpret_cast<ManagedSemaphoreBase*>(semaphore);

	destroySemaphore(managedSemaphore);
	m_semaphoreStorage->removeObject(managedSemaphore->getHandle());
}
