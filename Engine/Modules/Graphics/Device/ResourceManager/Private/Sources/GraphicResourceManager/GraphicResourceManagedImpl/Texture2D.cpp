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


void VT::GraphicResourceManager::deleteTexture2DViewInternal(ITexture2DView* view)
{
	EngineInstance::getInstance()->getEnvironment()->m_graphicDevice->destroyTexture2DView(view);
}

void VT::GraphicResourceManager::deleteTexture2DViewReference(Texture2DViewHandleID handleID)
{
	m_texture2DViews.removeElement(handleID);
}

VT::Texture2DViewReference VT::GraphicResourceManager::createTexture2DView(Texture2DReference texture, const TextureViewDesc& desc)
{
	assert(texture);

	ITexture2DView* view = getGraphicDevice()->createTexture2DView(texture->getResource(), desc);
	if (!view)
	{
		return nullptr;
	}

	Texture2DViewPool::NewElementInfo elementInfo = m_texture2DViews.addElementRaw();
	ManagedTexture2DViewResourceHandle* viewHandle
		= new (elementInfo.m_elementPtr) ManagedTexture2DViewResourceHandle(
			view, elementInfo.m_elementHandle.getKey(), texture.getObject()->getID());

	return viewHandle;
}

VT::Texture2DViewReference VT::GraphicResourceManager::getTexture2DView(Texture2DViewHandleID handle)
{
	return m_texture2DViews.getElement(handle);
}

bool VT::GraphicResourceManager::isValidTexture2DView(Texture2DViewHandleID handle)
{
	return m_texture2DViews.isValid(handle);
}