#include "GraphicResourceManager.h"

#include "Core/UtilitiesMacros.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "GraphicDevice/IGraphicDevice.h"

#include <cassert>

bool VT::GraphicResourceManager::init(Texture2DContainer* texture2DContainer)
{
	assert(!m_texture2DContainer);
	assert(texture2DContainer);

	m_texture2DContainer = texture2DContainer;

	VT_CHECK_INITIALIZATION(m_texture2DContainer && m_texture2DContainer->init());

	return true;
}

void VT::GraphicResourceManager::release()
{
	VT_SAFE_DESTROY_WITH_RELEASING(m_texture2DContainer);
}

VT::Texture2DContainer::NewResourceInfo VT::GraphicResourceManager::createTexture2D(const Texture2DDesc & desc)
{
	assert(m_texture2DContainer);

	VT::Texture2DContainer::NewResourceInfo info;
	m_texture2DContainer->addResource(info);

	//create texture from graphic device
	//EngineInstance::getInstance()->getEnvironment()->m_graphicDevice->createTexture2D(info.m_resourcePtr, desc);

	return info;
}

void VT::GraphicResourceManager::deleteTexture2D(Texture2DHandle handle)
{
	assert(m_texture2DContainer);
	m_texture2DContainer->removeResource(handle);
}

VT::ITexture2D* VT::GraphicResourceManager::getTexture2D(Texture2DHandle handle)
{
	assert(m_texture2DContainer);
	return m_texture2DContainer->getResource(handle);
}

bool VT::GraphicResourceManager::isValidTexture2D(Texture2DHandle handle) const
{
	assert(m_texture2DContainer);
	return m_texture2DContainer->isValidResourceHandle(handle);
}
