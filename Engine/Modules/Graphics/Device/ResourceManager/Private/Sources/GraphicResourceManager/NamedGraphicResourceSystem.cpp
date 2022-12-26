#include "NamedGraphicResourceSystem.h"

#include "CommonGraphicResourceManager.h"
#include "ShaderConverter/ShaderConverterArgs.h"

bool VT::NamedGraphicResourceSystem::init()
{
	VT_CHECK_INITIALIZATION(GraphicResourceManager::init())

	VT_CHECK_INITIALIZATION(m_namedVertexShaderPool.init(32, 0, 8))
	VT_CHECK_INITIALIZATION(m_namedPixelShaderPool.init(64, 0, 8))

	IResourceSystem* resourceSystem = getResourceSystem();
	if (resourceSystem)
	{
		resourceSystem->addResourceConverterArgsType<ShaderResourceConverterArgs>();
	}

	return true;
}

void VT::NamedGraphicResourceSystem::release()
{
	m_namedVertexShaderPool.release();
	m_namedPixelShaderPool.release();

	GraphicResourceManager::release();
}