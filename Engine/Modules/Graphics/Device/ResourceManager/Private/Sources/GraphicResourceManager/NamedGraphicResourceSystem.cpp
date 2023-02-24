#include "NamedGraphicResourceSystem.h"

#include "CommonGraphicResourceManager.h"
#include "ShaderConverter/ShaderConverterArgs.h"

bool VT::NamedGraphicResourceSystem::init()
{
	VT_CHECK_RETURN_FALSE(GraphicResourceManager::init())

	VT_CHECK_INITIALIZATION(m_namedVertexShaderPool.init(32, 0, 16))
	VT_CHECK_INITIALIZATION(m_namedPixelShaderPool.init(64, 0, 32))
	VT_CHECK_INITIALIZATION(m_namedComputeShaderPool.init(32, 0, 16))

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
	m_namedComputeShaderPool.release();

	GraphicResourceManager::release();
}