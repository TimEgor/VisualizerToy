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

	VT_CHECK_INITIALIZATION(initDevice(isSwapChainEnabled));

	return true;
}

void VT::ManagedGraphicDevice::ManagedGraphicDevice::release()
{
	releaseDevice();

	VT_SAFE_DESTROY_WITH_RELEASING(m_swapChainStorage);
	VT_SAFE_DESTROY_WITH_RELEASING(m_texture2DStorage);
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

	reinterpret_cast<ManagedSwapChainObject*>(newObjectInfo.m_objectPtr)->m_handle = newObjectInfo.m_objectHandle;

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