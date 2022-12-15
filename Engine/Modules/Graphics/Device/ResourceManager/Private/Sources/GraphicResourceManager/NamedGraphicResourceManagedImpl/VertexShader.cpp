#include "Common.h"

#include "Shaders/IShaders.h"
#include "ShaderConverter/ShaderConverterArgs.h"

void VT::NamedGraphicResourceSystem::deleteVertexShaderReference(FileNameID nameID)
{
	m_namedVertexPool.removeResource(nameID);
}

VT::VertexShaderResourceHandleReference VT::NamedGraphicResourceSystem::getVertexShader(VertexShaderHandleID handle)
{
	NamedVertexShaderPool::ResourcePoolHandleType poolHandle = handle;
	if (poolHandle.m_handle.getResourceType() == NAMELESS_RESOURCE_TYPE)
	{
		return GraphicResourceManager::getVertexShader(handle);
	}

	return getNamedVertexShader(handle);
}

bool VT::NamedGraphicResourceSystem::isValidVertexShader(VertexShaderHandleID handle) const
{
	NamedVertexShaderPool::ResourcePoolHandleType poolHandle = handle;
	if (poolHandle.m_handle.getResourceType() == NAMELESS_RESOURCE_TYPE)
	{
		return GraphicResourceManager::isValidVertexShader(handle);
	}

	return isValidNamedVertexShader(handle);
}

VT::VertexShaderResourceHandleReference VT::NamedGraphicResourceSystem::getNamedVertexShader(FileNameID handle)
{
	return m_namedVertexPool.getResource(handle);
}

bool VT::NamedGraphicResourceSystem::isValidNamedVertexShader(FileNameID handle) const
{
	return m_namedVertexPool.isValid(handle);
}

VT::VertexShaderResourceHandleReference VT::NamedGraphicResourceSystem::loadVertexShader(const FileName& shaderPath)
{
	FileNameID nameID = shaderPath.hash();
	NamedVertexShaderPool::ResourceAccessInfo shaderAccessor = m_namedVertexPool.getOrAddResource(nameID);
	if (shaderAccessor.m_isCreatingState)
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
					m_namedVertexPool.removeResource(nameID);
					return nullptr;
				}

				shaderAccessor.m_resource->m_resourcePtr = shader;
			}
		}
	}

	return shaderAccessor.m_resource;
}

VT::VertexShaderResourceHandleReference VT::NamedGraphicResourceSystem::loadVertexShaderAsync(
	const FileName& shaderPath, OnLoadedVertexShaderCallback callback)
{
	NamedVertexShaderPool::ResourceAccessInfo shaderAccessor = m_namedVertexPool.getOrAddResource(shaderPath);
	if (!shaderAccessor.m_isCreatingState)
	{
		if (callback)
		{
			callback(shaderAccessor.m_resource);
		}
	}
	else
	{
		IResourceSystem* resourceSystem = getResourceSystem();
		assert(resourceSystem);

		ResourceSystemConverterArgsReference args = resourceSystem->createResourceConverterArgs<ShaderResourceConverterArgs>(ShaderStageType::Vertex);
		resourceSystem->getResourceAsync(shaderPath,
			[shaderHandleReference = shaderAccessor.m_resource, onLoadedCallback = callback](const ResourceDataReference& resourceData)
			{
				if (resourceData && resourceData->getState() != ResourceState::INVALID)
				{
					const void* data = resourceData->getData();
					size_t dataSize = resourceData->getDataSize();

					if (data && dataSize > 0)
					{
						IVertexShader* shader = getGraphicDevice()->createVertexShader(data, dataSize);
						shaderHandleReference->m_resourcePtr = shader;
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

	return shaderAccessor.m_resource;
}