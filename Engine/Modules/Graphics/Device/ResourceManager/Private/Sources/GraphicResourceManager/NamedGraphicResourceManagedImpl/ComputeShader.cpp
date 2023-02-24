#include "Common.h"

#include "Shaders/IShaders.h"
#include "ShaderConverter/ShaderConverterArgs.h"

void VT::NamedGraphicResourceSystem::deleteComputeShaderReference(FileNameID nameID)
{
	m_namedComputeShaderPool.removeElement(nameID);
}

VT::ComputeShaderReference VT::NamedGraphicResourceSystem::getComputeShader(ComputeShaderHandleID handle)
{
	NamedComputeShaderPool::ElementPoolHandleType poolHandle = handle;
	if (poolHandle.m_handle.getElementType() == NAMELESS_ELEMENT_TYPE)
	{
		return GraphicResourceManager::getComputeShader(handle);
	}

	return getNamedComputeShader(handle);
}

bool VT::NamedGraphicResourceSystem::isValidComputeShader(ComputeShaderHandleID handle) const
{
	NamedComputeShaderPool::ElementPoolHandleType poolHandle = handle;
	if (poolHandle.m_handle.getElementType() == NAMELESS_ELEMENT_TYPE)
	{
		return GraphicResourceManager::isValidComputeShader(handle);
	}

	return isValidNamedComputeShader(handle);
}

VT::ComputeShaderReference VT::NamedGraphicResourceSystem::getNamedComputeShader(const GraphicResourceName& name)
{
	return getNamedComputeShader(name.hash());
}

VT::ComputeShaderReference VT::NamedGraphicResourceSystem::getNamedComputeShader(FileNameID handle)
{
	return m_namedComputeShaderPool.getElement(handle);
}

bool VT::NamedGraphicResourceSystem::isValidNamedComputeShader(FileNameID handle) const
{
	return m_namedComputeShaderPool.isValid(handle);
}

VT::ComputeShaderReference VT::NamedGraphicResourceSystem::loadComputeShader(const FileName& shaderPath)
{
	FileNameID nameID = shaderPath.hash();
	NamedComputeShaderPool::ElementAccessInfo shaderAccessor = m_namedComputeShaderPool.getOrAddElement(nameID);
	if (shaderAccessor.m_isNew)
	{
		IResourceSystem* resourceSystem = getResourceSystem();
		assert(resourceSystem);

		ResourceSystemConverterArgsReference args = resourceSystem->createResourceConverterArgs<ShaderResourceConverterArgs>(ShaderType::Compute);
		ResourceDataReference resourceData = resourceSystem->getResource(shaderPath, args);

		if (resourceData && resourceData->getState() != ResourceState::INVALID)
		{
			const void* data = resourceData->getData();
			size_t dataSize = resourceData->getDataSize();

			if (data && dataSize > 0)
			{
				IComputeShader* shader = getGraphicDevice()->createComputeShader(data, dataSize);
				if (!shader)
				{
					return nullptr;
				}

				shaderAccessor.m_element.getObjectCast<ManagedComputeShaderGraphicObjectHandle>()->setHandleObject(shader);
			}
		}


		if (!shaderAccessor.m_element->getTypedObject())
		{
			return nullptr;
		}
	}

	return shaderAccessor.m_element;
}

VT::ComputeShaderReference VT::NamedGraphicResourceSystem::loadComputeShaderAsync(
	const FileName& shaderPath, OnLoadedComputeShaderCallback callback)
{
	NamedComputeShaderPool::ElementAccessInfo shaderAccessor = m_namedComputeShaderPool.getOrAddElement(shaderPath);
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

		ResourceSystemConverterArgsReference args = resourceSystem->createResourceConverterArgs<ShaderResourceConverterArgs>(ShaderType::Compute);
		resourceSystem->getResourceAsync(shaderPath,
			[shaderHandleReference = shaderAccessor.m_element, onLoadedCallback = callback](const ResourceDataReference& resourceData)
			{
				if (resourceData && resourceData->getState() != ResourceState::INVALID)
				{
					const void* data = resourceData->getData();
					size_t dataSize = resourceData->getDataSize();

					if (data && dataSize > 0)
					{
						IComputeShader* shader = getGraphicDevice()->createComputeShader(data, dataSize);
						shaderHandleReference.getObjectCast<ManagedComputeShaderGraphicObjectHandle>()->setHandleObject(shader);
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