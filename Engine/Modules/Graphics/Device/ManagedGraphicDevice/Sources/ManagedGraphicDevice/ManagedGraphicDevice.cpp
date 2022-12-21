#include "ManagedGraphicDevice.h"

#include "Core/UtilitiesMacros.h"

bool VT::ManagedGraphicDevice::ManagedGraphicDevice::init(bool isSwapChainEnabled)
{
	if (isSwapChainEnabled)
	{
		m_swapChainStorage = createSwapChainStorage();
		VT_CHECK_INITIALIZATION(m_swapChainStorage && m_swapChainStorage->init(4, 0, 0));
	}

	m_texture2DStorage = createTexture2DStorage();
	VT_CHECK_INITIALIZATION(m_texture2DStorage && m_texture2DStorage->init(256, 1, 64));

	m_vertexShaderStorage = createVertexShaderStorage();
	VT_CHECK_INITIALIZATION(m_vertexShaderStorage && m_vertexShaderStorage->init(64, 1, 16));

	m_pixelShaderStorage = createPixelShaderStorage();
	VT_CHECK_INITIALIZATION(m_pixelShaderStorage && m_pixelShaderStorage->init(128, 1, 16));

	m_pipelineStateStorage = createPipelineStateStorage();
	VT_CHECK_INITIALIZATION(m_pipelineStateStorage && m_pipelineStateStorage->init(256, 1, 64));

	m_renderPassStorage = createRenderPassStorage();
	VT_CHECK_INITIALIZATION(m_renderPassStorage && m_renderPassStorage->init(32, 0, 16));

	VT_CHECK_INITIALIZATION(initDevice(isSwapChainEnabled));

	return true;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::release()
{
	releaseDevice();

	VT_SAFE_DESTROY_WITH_RELEASING(m_swapChainStorage);
	VT_SAFE_DESTROY_WITH_RELEASING(m_texture2DStorage);
	VT_SAFE_DESTROY_WITH_RELEASING(m_vertexShaderStorage);
	VT_SAFE_DESTROY_WITH_RELEASING(m_pixelShaderStorage);
	VT_SAFE_DESTROY_WITH_RELEASING(m_pipelineStateStorage);
	VT_SAFE_DESTROY_WITH_RELEASING(m_renderPassStorage);
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

void VT::ManagedGraphicDevice::ManagedGraphicDevice::destroyTexture2D(ITexture2D* texture)
{
	assert(texture);
	assert(m_texture2DStorage);

	ManagedTexture2DBase* managedTexture = reinterpret_cast<ManagedTexture2DBase*>(texture);

	destroyTexture2D(managedTexture);
	m_texture2DStorage->removeObject(managedTexture->getHandle());
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
	const PipelineStateInfo& pipelineStateInfo, const IRenderPass* renderPass)
{
	assert(m_pipelineStateStorage);

	ManagedPipelineStateStorageInfoBase::NewObjectInfo newObjectInfo = m_pipelineStateStorage->addNewObject();
	if (!createPipelineState(newObjectInfo.m_objectPtr, pipelineStateInfo, renderPass))
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

VT::IRenderPass* VT::ManagedGraphicDevice::ManagedGraphicDevice::createRenderPass(const RenderPassInfo& info)
{
	assert(m_renderPassStorage);

	ManagedRenderPassStorageInfoBase::NewObjectInfo newObjectInfo = m_renderPassStorage->addNewObject();
	if (!createRenderPass(newObjectInfo.m_objectPtr, info))
	{
		m_renderPassStorage->removeObject(newObjectInfo.m_objectHandle);
		return nullptr;
	}

	newObjectInfo.m_objectPtr->m_handle = newObjectInfo.m_objectHandle;

	return newObjectInfo.m_objectPtr;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::destroyRenderPass(IRenderPass* pass)
{
	assert(pass);
	assert(m_renderPassStorage);

	ManagedRenderPassBase* managedPass = reinterpret_cast<ManagedRenderPassBase*>(pass);

	destroyRenderPass(managedPass);
	m_renderPassStorage->removeObject(managedPass->getHandle());
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