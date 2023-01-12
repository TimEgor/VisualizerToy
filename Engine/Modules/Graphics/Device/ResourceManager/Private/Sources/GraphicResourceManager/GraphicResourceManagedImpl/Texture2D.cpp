#include "Common.h"

void VT::GraphicResourceManager::deleteTexture2DInternal(ITexture2D* texture)
{
	EngineInstance::getInstance()->getEnvironment()->m_graphicDevice->destroyTexture2D(texture);
}

void VT::GraphicResourceManager::deleteTexture2DReference(Texture2DHandleID handleID)
{
	m_textures2D.removeElement(handleID);
}

VT::Texture2DReference VT::GraphicResourceManager::createTexture2D(const Texture2DDesc& desc)
{
	return Texture2DReference();
}

VT::Texture2DReference VT::GraphicResourceManager::getTexture2D(Texture2DHandleID handle)
{
	return m_textures2D.getElement(handle);
}

bool VT::GraphicResourceManager::isValidTexture2D(Texture2DHandleID handle) const
{
	return m_textures2D.isValid(handle);
}