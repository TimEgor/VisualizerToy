#include "CommonGraphicResourceManager.h"

#include "ShaderConverter/ShaderConverterArgs.h"

bool VT::GraphicResourceManager::init()
{
	VT_CHECK_INITIALIZATION(m_textures2D.init(256, 1, 64));

	VT_CHECK_INITIALIZATION(m_pixelShaders.init(128, 1, 16));
	VT_CHECK_INITIALIZATION(m_vertexShaders.init(64, 1, 16));

	IResourceSystem* resourceSystem = getResourceSystem();
	if (resourceSystem)
	{
		resourceSystem->addResourceConverterArgsType<ShaderResourceConverterArgs>();
	}

	return true;
}

void VT::GraphicResourceManager::release()
{
	m_textures2D.release();

	m_vertexShaders.release();
	m_pixelShaders.release();
}