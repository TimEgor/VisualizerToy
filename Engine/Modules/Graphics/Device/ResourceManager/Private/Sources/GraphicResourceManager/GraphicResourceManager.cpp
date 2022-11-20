#include "GraphicResourceManager.h"

#include "Core/UtilitiesMacros.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "GraphicDevice/IGraphicDevice.h"

#include <cassert>

void VT::GraphicResourceManager::deleteTexture2DInternal(ITexture2D* texture)
{
	EngineInstance::getInstance()->getEnvironment()->m_graphicDevice->destroyTexture2D(texture);
}

bool VT::GraphicResourceManager::init()
{
	VT_CHECK_INITIALIZATION(m_textures2D.init(256, 1, 64));

	return true;
}

void VT::GraphicResourceManager::release()
{
	m_textures2D.release();
}

VT::Texture2DResourceHandleReference VT::GraphicResourceManager::createTexture2D(const Texture2DDesc & desc)
{
	return Texture2DResourceHandleReference();
}

void VT::GraphicResourceManager::deleteTexture2D(Texture2DResourceHandleConstReference textureReference)
{
	m_textures2D.removeElement(textureReference->m_resourceHandle);
}

void VT::GraphicResourceManager::deleteTexture2D(Texture2DHandleID handle)
{
	m_textures2D.removeElement(handle);
}

VT::Texture2DResourceHandleReference VT::GraphicResourceManager::getTexture2D(Texture2DHandleID handle)
{
	return m_textures2D.getElement(handle);
}

bool VT::GraphicResourceManager::isValidTexture2D(Texture2DHandleID handle) const
{
	return m_textures2D.isValid(handle);
}
