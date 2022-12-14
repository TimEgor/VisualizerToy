#include "Common.h"

#include "Shaders/IShaders.h"
#include "ShaderConverter/ShaderConverterArgs.h"

void VT::NamedGraphicResourceSystem::deletePixelShaderReference(FileNameID nameID)
{
	m_namedPixelPool.removeResource(nameID);
}

VT::PixelShaderResourceHandleReference VT::NamedGraphicResourceSystem::getPixelShader(PixelShaderHandleID handle)
{
	NamedPixelShaderPool::ResourcePoolHandleType poolHandle = handle;
	if (poolHandle.m_handle.getResourceType() == NAMELESS_RESOURCE_TYPE)
	{
		return GraphicResourceManager::getPixelShader(handle);
	}

	return getNamedPixelShader(handle);
}

bool VT::NamedGraphicResourceSystem::isValidPixelShader(PixelShaderHandleID handle) const
{
	NamedPixelShaderPool::ResourcePoolHandleType poolHandle = handle;
	if (poolHandle.m_handle.getResourceType() == NAMELESS_RESOURCE_TYPE)
	{
		return GraphicResourceManager::isValidPixelShader(handle);
	}

	return isValidNamedPixelShader(handle);
}

VT::PixelShaderResourceHandleReference VT::NamedGraphicResourceSystem::getNamedPixelShader(FileNameID handle)
{
	return m_namedPixelPool.getResource(handle);
}

bool VT::NamedGraphicResourceSystem::isValidNamedPixelShader(FileNameID handle) const
{
	return m_namedPixelPool.isValid(handle);
}

VT::PixelShaderResourceHandleReference VT::NamedGraphicResourceSystem::loadPixelShader(const FileName& shaderPath)
{
	PixelShaderResourceHandleReference shaderRef = m_namedPixelPool.getResource(shaderPath);
	if (shaderRef)
	{
		return shaderRef;
	}

	IResourceSystem* resourceSystem = getResourceSystem();
	assert(resourceSystem);

	ResourceSystemConverterArgsReference args = resourceSystem->createResourceConverterArgs<ShaderResourceConverterArgs>(ShaderStageType::Pixel);
	ResourceDataReference resourceData = resourceSystem->getResource(shaderPath, args);

	if (resourceData && resourceData->getState() != ResourceState::INVALID)
	{
		const void* data = resourceData->getData();
		size_t dataSize = resourceData->getDataSize();

		if (data && dataSize > 0)
		{
			FileNameID nameID = shaderPath.hash();

			IPixelShader* shader = getGraphicDevice()->createPixelShader(data, dataSize);
			if (!shader)
			{
				return nullptr;
			}

			NamedPixelShaderPool::NewResourceInfo elementInfo = m_namedPixelPool.addResource(nameID);
			NamedManagedPixelShaderResourceHandle* shaderHandle
				= new (elementInfo.m_elementPtr) NamedManagedPixelShaderResourceHandle(shader, elementInfo.m_elementHandle.getKey(), nameID);

			return shaderHandle;
		}
	}

	return nullptr;
}

VT::PixelShaderResourceHandleReference VT::NamedGraphicResourceSystem::loadPixelShaderAsync(
	const FileName& shaderPath, OnLoadedPixelShaderCallback callback)
{
	PixelShaderResourceHandleReference shaderRef = m_namedPixelPool.getResource(shaderPath);
	if (shaderRef)
	{
		if (callback)
		{
			callback(shaderRef);
		}

		return shaderRef;
	}

	const FileNameID nameID = shaderPath.hash();

	NamedPixelShaderPool::NewResourceInfo elementInfo = m_namedPixelPool.addResource(nameID);
	NamedManagedPixelShaderResourceHandle* shaderHandle
		= new (elementInfo.m_elementPtr) NamedManagedPixelShaderResourceHandle(nullptr, elementInfo.m_elementHandle.getKey(), nameID);
	PixelShaderResourceHandleReference shaderHandleReference = shaderHandle;

	IResourceSystem* resourceSystem = getResourceSystem();
	assert(resourceSystem);

	ResourceSystemConverterArgsReference args = resourceSystem->createResourceConverterArgs<ShaderResourceConverterArgs>(ShaderStageType::Pixel);
	resourceSystem->getResourceAsync(shaderPath,
		[shaderHandleReference, onLoadedCallback = callback](const ResourceDataReference& resourceData)
		{
			if (resourceData && resourceData->getState() != ResourceState::INVALID)
			{
				const void* data = resourceData->getData();
				size_t dataSize = resourceData->getDataSize();

				if (data && dataSize > 0)
				{
					IPixelShader* shader = getGraphicDevice()->createPixelShader(data, dataSize);
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