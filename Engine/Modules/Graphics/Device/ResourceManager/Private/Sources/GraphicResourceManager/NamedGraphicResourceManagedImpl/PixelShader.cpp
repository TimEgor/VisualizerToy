#include "Common.h"

#include "Shaders/IShaders.h"
#include "ShaderConverter/ShaderConverterArgs.h"

void VT::NamedGraphicResourceSystem::deletePixelShaderReference(FileNameID nameID)
{
	m_namedPixelPool.removeResource(nameID);
}

VT::PixelShaderReference VT::NamedGraphicResourceSystem::getPixelShader(PixelShaderHandleID handle)
{
	NamedPixelShaderPool::ResourcePoolHandle poolHandle = handle;
	if (poolHandle.m_handle.getResourceType() == NAMELESS_RESOURCE_TYPE)
	{
		return GraphicResourceManager::getPixelShader(handle);
	}

	return getNamedPixelShader(handle);
}

bool VT::NamedGraphicResourceSystem::isValidPixelShader(PixelShaderHandleID handle) const
{
	NamedPixelShaderPool::ResourcePoolHandle poolHandle = handle;
	if (poolHandle.m_handle.getResourceType() == NAMELESS_RESOURCE_TYPE)
	{
		return GraphicResourceManager::isValidPixelShader(handle);
	}

	return isValidNamedPixelShader(handle);
}

VT::PixelShaderReference VT::NamedGraphicResourceSystem::getNamedPixelShader(const GraphicResourceName& name)
{
	return getNamedPixelShader(name.hash());
}

VT::PixelShaderReference VT::NamedGraphicResourceSystem::getNamedPixelShader(FileNameID handle)
{
	return m_namedPixelPool.getResource(handle);
}

bool VT::NamedGraphicResourceSystem::isValidNamedPixelShader(FileNameID handle) const
{
	return m_namedPixelPool.isValid(handle);
}

VT::PixelShaderReference VT::NamedGraphicResourceSystem::loadPixelShader(const FileName& shaderPath)
{
	FileNameID nameID = shaderPath.hash();
	NamedPixelShaderPool::ResourceAccessInfo shaderAccessor = m_namedPixelPool.getOrAddResource(nameID);
	if (shaderAccessor.m_isNew)
	{
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
				IPixelShader* shader = getGraphicDevice()->createPixelShader(data, dataSize);
				if (!shader)
				{
					m_namedPixelPool.removeResource(nameID);
					return nullptr;
				}

				shaderAccessor.m_element.getObjectCast<ManagedPixelShaderResourceHandle>()->setResource(shader);
			}
		}
	}

	return shaderAccessor.m_element;
}

VT::PixelShaderReference VT::NamedGraphicResourceSystem::loadPixelShaderAsync(
	const FileName& shaderPath, OnLoadedPixelShaderCallback callback)
{
	NamedPixelShaderPool::ResourceAccessInfo shaderAccessor = m_namedPixelPool.getOrAddResource(shaderPath);
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

		ResourceSystemConverterArgsReference args = resourceSystem->createResourceConverterArgs<ShaderResourceConverterArgs>(ShaderStageType::Pixel);
		resourceSystem->getResourceAsync(shaderPath,
			[shaderHandleReference = shaderAccessor.m_element, onLoadedCallback = callback](const ResourceDataReference& resourceData)
			{
				if (resourceData && resourceData->getState() != ResourceState::INVALID)
				{
					const void* data = resourceData->getData();
					size_t dataSize = resourceData->getDataSize();

					if (data && dataSize > 0)
					{
						IPixelShader* shader = getGraphicDevice()->createPixelShader(data, dataSize);
						shaderHandleReference.getObjectCast<ManagedPixelShaderResourceHandle>()->setResource(shader);
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