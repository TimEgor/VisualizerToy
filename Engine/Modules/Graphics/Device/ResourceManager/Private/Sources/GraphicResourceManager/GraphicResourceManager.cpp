#include "GraphicResourceManager.h"

#include "Core/UtilitiesMacros.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "GraphicDevice/IGraphicDevice.h"

#include <cassert>

bool VT::GraphicResourceManager::isContainerCollectionEmpty(const ResourceManagerContainerCollection& collection)
{
	return !collection.m_swapChainContainer
		&& !collection.m_texture2DContainer;
}

bool VT::GraphicResourceManager::init(const ResourceManagerContainerCollection& containers, bool isSwapChainEnabled)
{
	assert(isContainerCollectionEmpty(m_containers));

	m_containers = containers;

	if (isSwapChainEnabled)
	{
		VT_CHECK_INITIALIZATION(m_containers.m_swapChainContainer && m_containers.m_swapChainContainer->init());
	}

	VT_CHECK_INITIALIZATION(m_containers.m_texture2DContainer && m_containers.m_texture2DContainer->init());

	return true;
}

void VT::GraphicResourceManager::release()
{
	VT_SAFE_DESTROY_WITH_RELEASING(m_containers.m_swapChainContainer);
	VT_SAFE_DESTROY_WITH_RELEASING(m_containers.m_texture2DContainer);
}

VT::SwapChainContainer::NewResourceInfo VT::GraphicResourceManager::createSwapChain(const SwapChainDesc& desc, const IWindow* window)
{
	assert(m_containers.m_swapChainContainer);
	assert(m_containers.m_texture2DContainer);

	VT::SwapChainContainer::NewResourceInfo newSwapChainInfo;
	m_containers.m_swapChainContainer->addResource(newSwapChainInfo);

	EngineInstance::getInstance()->getEnvironment()->m_graphicDevice->createSwapChain(desc, window, newSwapChainInfo.m_resourcePtr);

	return newSwapChainInfo;
}

void VT::GraphicResourceManager::deleteSwapChain(SwapChainHandle handle)
{
	assert(m_containers.m_swapChainContainer);
	m_containers.m_swapChainContainer->removeResource(handle);
}

VT::ISwapChain* VT::GraphicResourceManager::getSwapChain(SwapChainHandle handle)
{
	assert(m_containers.m_swapChainContainer);
	return m_containers.m_swapChainContainer->getResource(handle);
}

bool VT::GraphicResourceManager::isValidSwapChain(SwapChainHandle handle) const
{
	assert(m_containers.m_swapChainContainer);
	return m_containers.m_swapChainContainer->isValidResourceHandle(handle);
}

VT::Texture2DContainer::NewResourceInfo VT::GraphicResourceManager::createTexture2D(const Texture2DDesc & desc)
{
	assert(m_containers.m_texture2DContainer);

	VT::Texture2DContainer::NewResourceInfo info;
	m_containers.m_texture2DContainer->addResource(info);

	//create texture from graphic device
	//EngineInstance::getInstance()->getEnvironment()->m_graphicDevice->createTexture2D(info.m_resourcePtr, desc);

	return info;
}

void VT::GraphicResourceManager::deleteTexture2D(Texture2DHandle handle)
{
	assert(m_containers.m_texture2DContainer);
	m_containers.m_texture2DContainer->removeResource(handle);
}

VT::ITexture2D* VT::GraphicResourceManager::getTexture2D(Texture2DHandle handle)
{
	assert(m_containers.m_texture2DContainer);
	return m_containers.m_texture2DContainer->getResource(handle);
}

bool VT::GraphicResourceManager::isValidTexture2D(Texture2DHandle handle) const
{
	assert(m_containers.m_texture2DContainer);
	return m_containers.m_texture2DContainer->isValidResourceHandle(handle);
}
