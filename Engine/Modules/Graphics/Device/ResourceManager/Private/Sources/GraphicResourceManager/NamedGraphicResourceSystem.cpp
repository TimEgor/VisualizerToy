#include "NamedGraphicResourceSystem.h"

#include "CommonGraphicResourceManager.h"
#include "ShaderConverter/ShaderConverterArgs.h"

bool VT::NamedGraphicResourceSystem::init()
{
	VT_CHECK_INITIALIZATION(GraphicResourceManager::init())

	VT_CHECK_INITIALIZATION(m_namedVertexPool.init(32, 0, 8))
	VT_CHECK_INITIALIZATION(m_namedPixelPool.init(64, 0, 8))

	IResourceSystem* resourceSystem = getResourceSystem();
	if (resourceSystem)
	{
		resourceSystem->addResourceConverterArgsType<ShaderResourceConverterArgs>();
	}

	return true;
}

void VT::NamedGraphicResourceSystem::release()
{
	m_namedVertexPool.release();
	m_namedPixelPool.release();

	GraphicResourceManager::release();
}