#include "Common.h"

#include "Shaders/IShaders.h"
#include "ShaderConverter/ShaderConverterArgs.h"

void VT::NamedGraphicResourceSystem::deletePixelShaderReference(FileNameID nameID)
{
	m_namedPixelShaderPool.removeElement(nameID);
}

VT::PixelShaderReference VT::NamedGraphicResourceSystem::getPixelShader(PixelShaderHandleID handle)
{
	NamedPixelShaderPool::ElementPoolHandleType poolHandle = handle;
	if (poolHandle.m_handle.getElementType() == NAMELESS_ELEMENT_TYPE)
	{
		return GraphicResourceManager::getPixelShader(handle);
	}

	return getNamedPixelShader(handle);
}

bool VT::NamedGraphicResourceSystem::isValidPixelShader(PixelShaderHandleID handle) const
{
	NamedPixelShaderPool::ElementPoolHandleType poolHandle = handle;
	if (poolHandle.m_handle.getElementType() == NAMELESS_ELEMENT_TYPE)
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
	return m_namedPixelShaderPool.getElement(handle);
}

bool VT::NamedGraphicResourceSystem::isValidNamedPixelShader(FileNameID handle) const
{
	return m_namedPixelShaderPool.isValid(handle);
}

VT::PixelShaderReference VT::NamedGraphicResourceSystem::loadPixelShader(const FileName& shaderPath)
{
	FileNameID nameID = shaderPath.hash();
	NamedPixelShaderPool::ElementAccessInfo shaderAccessor = m_namedPixelShaderPool.getOrAddElement(nameID);
	if (shaderAccessor.m_isNew)
	{
		IResourceSystem* resourceSystem = getResourceSystem();
		assert(resourceSystem);

		ResourceSystemConverterArgsReference args = resourceSystem->createResourceConverterArgs<ShaderResourceConverterArgs>(ShaderType::Pixel);
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
					return nullptr;
				}

				shaderAccessor.m_element.getObjectCast<ManagedPixelShaderGraphicObjectHandle>()->setHandleObject(shader);
			}
		}


		if (!shaderAccessor.m_element->getTypedObject())
		{
			return nullptr;
		}
	}

	return shaderAccessor.m_element;
}

VT::PixelShaderReference VT::NamedGraphicResourceSystem::loadPixelShaderAsync(
	const FileName& shaderPath, OnLoadedPixelShaderCallback callback)
{
	NamedPixelShaderPool::ElementAccessInfo shaderAccessor = m_namedPixelShaderPool.getOrAddElement(shaderPath);
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

		ResourceSystemConverterArgsReference args = resourceSystem->createResourceConverterArgs<ShaderResourceConverterArgs>(ShaderType::Pixel);
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
						shaderHandleReference.getObjectCast<ManagedPixelShaderGraphicObjectHandle>()->setHandleObject(shader);
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