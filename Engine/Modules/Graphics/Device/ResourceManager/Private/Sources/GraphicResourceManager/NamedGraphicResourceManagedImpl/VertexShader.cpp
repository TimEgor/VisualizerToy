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
	VertexShaderResourceHandleReference shaderRef = m_namedVertexPool.getResource(shaderPath);
	if (shaderRef)
	{
		return shaderRef;
	}

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
			FileNameID nameID = shaderPath.hash();

			IVertexShader* shader = getGraphicDevice()->createVertexShader(data, dataSize);
			if (!shader)
			{
				return nullptr;
			}

			NamedVertexShaderPool::NewResourceInfo elementInfo = m_namedVertexPool.addResource(nameID);
			NamedManagedVertexShaderResourceHandle* shaderHandle
				= new (elementInfo.m_elementPtr) NamedManagedVertexShaderResourceHandle(shader, elementInfo.m_elementHandle.getKey(), nameID);

			return shaderHandle;
		}
	}

	return nullptr;
}

VT::VertexShaderResourceHandleReference VT::NamedGraphicResourceSystem::loadVertexShaderAsync(
	const FileName& shaderPath, OnLoadedVertexShaderCallback callback)
{
	VertexShaderResourceHandleReference shaderRef = m_namedVertexPool.getResource(shaderPath);
	if (shaderRef)
	{
		if (callback)
		{
			callback(shaderRef);
		}

		return shaderRef;
	}

	const FileNameID nameID = shaderPath.hash();

	NamedVertexShaderPool::NewResourceInfo elementInfo = m_namedVertexPool.addResource(nameID);
	NamedManagedVertexShaderResourceHandle* shaderHandle
		= new (elementInfo.m_elementPtr) NamedManagedVertexShaderResourceHandle(nullptr, elementInfo.m_elementHandle.getKey(), nameID);
	VertexShaderResourceHandleReference shaderHandleReference = shaderHandle;

	IResourceSystem* resourceSystem = getResourceSystem();
	assert(resourceSystem);

	ResourceSystemConverterArgsReference args = resourceSystem->createResourceConverterArgs<ShaderResourceConverterArgs>(ShaderStageType::Vertex);
	resourceSystem->getResourceAsync(shaderPath,
		[shaderHandleReference, onLoadedCallback = callback](const ResourceDataReference& resourceData)
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

	return shaderHandleReference;
}