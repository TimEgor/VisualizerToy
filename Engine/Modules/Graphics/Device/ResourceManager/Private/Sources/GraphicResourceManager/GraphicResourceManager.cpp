#include "GraphicResourceManager.h"

#include "Core/UtilitiesMacros.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "GraphicDevice/IGraphicDevice.h"

#include <cassert>

bool VT::GraphicResourceManager::init()
{
	VT_CHECK_INITIALIZATION(m_textures2D.init(256, 1, 64));

	return true;
}

void VT::GraphicResourceManager::release()
{
	m_textures2D.release();
}

VT::Texture2DResourceInfo VT::GraphicResourceManager::createTexture2D(const Texture2DDesc & desc)
{
	return Texture2DResourceInfo();
}

void VT::GraphicResourceManager::deleteTexture2D(Texture2DHandle handle)
{
	m_textures2D.removeElement(handle);
}

VT::ITexture2D* VT::GraphicResourceManager::getTexture2D(Texture2DHandle handle)
{
	ITexture2D** ptr = m_textures2D.getElement(handle);
	return ptr ? *ptr : nullptr;
}

bool VT::GraphicResourceManager::isValidTexture2D(Texture2DHandle handle) const
{
	return m_textures2D.isValid(handle);
}
