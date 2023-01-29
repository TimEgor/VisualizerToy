#include "Common.h"

void VT::GraphicResourceManager::deleteTexture2DInternal(ITexture2D* texture)
{
	EngineInstance::getInstance()->getEnvironment()->m_graphicDevice->destroyTexture2D(texture);
}

void VT::GraphicResourceManager::deleteTexture2DReference(TextureHandleID handleID)
{
	m_textures2D.removeElement(handleID);
}

VT::Texture2DReference VT::GraphicResourceManager::createTexture2D(const Texture2DDesc& desc)
{
	return Texture2DReference();
}

VT::Texture2DReference VT::GraphicResourceManager::getTexture2D(TextureHandleID handle)
{
	return m_textures2D.getElement(handle);
}

bool VT::GraphicResourceManager::isValidTexture2D(TextureHandleID handle) const
{
	return m_textures2D.isValid(handle);
}