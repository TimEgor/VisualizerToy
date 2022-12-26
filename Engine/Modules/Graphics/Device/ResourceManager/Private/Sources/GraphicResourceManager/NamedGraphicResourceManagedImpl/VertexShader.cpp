#include "Common.h"

#include "Shaders/IShaders.h"
#include "ShaderConverter/ShaderConverterArgs.h"

void VT::NamedGraphicResourceSystem::deleteVertexShaderReference(FileNameID nameID)
{
	m_namedVertexShaderPool.removeResource(nameID);
}

VT::VertexShaderReference VT::NamedGraphicResourceSystem::getVertexShader(VertexShaderHandleID handle)
{
	NamedVertexShaderPool::ResourcePoolHandle poolHandle = handle;
	if (poolHandle.m_handle.getResourceType() == NAMELESS_RESOURCE_TYPE)
	{
		return GraphicResourceManager::getVertexShader(handle);
	}

	return getNamedVertexShader(handle);
}

bool VT::NamedGraphicResourceSystem::isValidVertexShader(VertexShaderHandleID handle) const
{
	NamedVertexShaderPool::ResourcePoolHandle poolHandle = handle;
	if (poolHandle.m_handle.getResourceType() == NAMELESS_RESOURCE_TYPE)
	{
		return GraphicResourceManager::isValidVertexShader(handle);
	}

	return isValidNamedVertexShader(handle);
}

VT::VertexShaderReference VT::NamedGraphicResourceSystem::getNamedVertexShader(const GraphicResourceName& name)
{
	return getNamedVertexShader(name.hash());
}

VT::VertexShaderReference VT::NamedGraphicResourceSystem::getNamedVertexShader(FileNameID handle)
{
	return m_namedVertexShaderPool.getResource(handle);
}

bool VT::NamedGraphicResourceSystem::isValidNamedVertexShader(FileNameID handle) const
{
	return m_namedVertexShaderPool.isValid(handle);
}

VT::VertexShaderReference VT::NamedGraphicResourceSystem::loadVertexShader(const FileName& shaderPath)
{
	FileNameID nameID = shaderPath.hash();
	NamedVertexShaderPool::ResourceAccessInfo shaderAccessor = m_namedVertexShaderPool.getOrAddResource(nameID);
	if (shaderAccessor.m_isNew)
	{
		IResourceSystem* resourceSystem = getResourceSystem();
		assert(resourceSystem);

		ResourceSystemConverterArgsReference args = resourceSystem->createResourceConverterArgs<ShaderResourceConverterArgs>(ShaderStageType::Vertex);
		ResourceDataReference resourceData = resourceSystem->getResource(shaderPath, args);

		if (resourceData && resourceData->getState() != ResourceState::INVALID)
		{
			const void* data = resourceData->getData();
			size_t dataSize = resourceData->getDataSize();

			if (data && dataSize > 0)
			{
				IVertexShader* shader = getGraphicDevice()->createVertexShader(data, dataSize);
				if (!shader)
				{
					m_namedVertexShaderPool.removeResource(nameID);
					return nullptr;
				}

				shaderAccessor.m_element.getObjectCast<ManagedVertexShaderResourceHandle>()->setResource(shader);
			}
		}
	}

	return shaderAccessor.m_element;
}

VT::VertexShaderReference VT::NamedGraphicResourceSystem::loadVertexShaderAsync(
	const FileName& shaderPath, OnLoadedVertexShaderCallback callback)
{
	NamedVertexShaderPool::ResourceAccessInfo shaderAccessor = m_namedVertexShaderPool.getOrAddResource(shaderPath);
	if (!shaderAccessor.m_isNew)
	{
		if (callback)
		{
			callback(shaderAccessor.m_element);
		}
	}
	else
	{
		IResourceSystem* resourceSystem = getResourceSystem();
		assert(resourceSystem);

		ResourceSystemConverterArgsReference args = resourceSystem->createResourceConverterArgs<ShaderResourceConverterArgs>(ShaderStageType::Vertex);
		resourceSystem->getResourceAsync(shaderPath,
			[shaderHandleReference = shaderAccessor.m_element, onLoadedCallback = callback](const ResourceDataReference& resourceData)
			{
				if (resourceData && resourceData->getState() != ResourceState::INVALID)
				{
					const void* data = resourceData->getData();
					size_t dataSize = resourceData->getDataSize();

					if (data && dataSize > 0)
					{
						IVertexShader* shader = getGraphicDevice()->createVertexShader(data, dataSize);
						shaderHandleReference.getObjectCast<ManagedVertexShaderResourceHandle>()->setResource(shader);
					}
				}

				if (onLoadedCallback)
				{
					onLoadedCallback(shaderHandleReference);
				}
			},
			args
		);
	}

	return shaderAccessor.m_element;
}