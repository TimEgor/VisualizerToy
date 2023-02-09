#include "Common.h"

void VT::GraphicResourceManager::deleteTexture2DInternal(ITexture2D* texture)
{
	EngineInstance::getInstance()->getEnvironment()->m_graphicDevice->destroyTexture2D(texture);
}

void VT::GraphicResourceManager::deleteTexture2DReference(TextureHandleID handleID)
{
	m_textures2D.removeElement(handleID);
}

VT::Texture2DReference VT::GraphicResourceManager::createTexture2D(const Texture2DDesc& desc, GraphicStateValueType initialState)
{
	ITexture2D* texture = getGraphicDevice()->createTexture2D(desc, initialState);
	if (!texture)
	{
		return nullptr;
	}

	Texture2DPool::NewElementInfo elementInfo = m_textures2D.addElementRaw();
	ManagedTexture2DResourceHandle* textureHandle
		= new (elementInfo.m_elementPtr) ManagedTexture2DResourceHandle(texture, elementInfo.m_elementHandle.getKey());

	return textureHandle;
}

VT::Texture2DReference VT::GraphicResourceManager::getTexture2D(TextureHandleID handle)
{
	return m_textures2D.getElement(handle);
}

bool VT::GraphicResourceManager::isValidTexture2D(TextureHandleID handle) const
{
	return m_textures2D.isValid(handle);
}