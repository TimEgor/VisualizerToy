#include "Common.h"

void VT::GraphicResourceManager::deleteTexture2DInternal(ITexture2D* texture)
{
	EngineInstance::getInstance()->getEnvironment()->m_graphicDevice->destroyTexture2D(texture);
}

void VT::GraphicResourceManager::deleteTexture2DReference(Texture2DPool::HandleElementType handle)
{
	m_textures2D.removeElement(handle);
}

VT::Texture2DResourceHandleReference VT::GraphicResourceManager::createTexture2D(const Texture2DDesc& desc)
{
	return Texture2DResourceHandleReference();
}

VT::Texture2DResourceHandleReference VT::GraphicResourceManager::getTexture2D(Texture2DHandleID handle)
{
	return m_textures2D.getElement(handle);
}

bool VT::GraphicResourceManager::isValidTexture2D(Texture2DHandleID handle) const
{
	return m_textures2D.isValid(handle);
}