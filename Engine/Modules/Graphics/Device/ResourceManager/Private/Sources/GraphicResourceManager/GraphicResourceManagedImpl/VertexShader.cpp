#include "GraphicResourceManager/CommonGraphicResourceManager.h"

#include "ShaderConverter/ShaderConverterArgs.h"

void VT::GraphicResourceManager::deleteVertexShaderInternal(IVertexShader* shader)
{
	getGraphicDevice()->destroyVertexShader(shader);
}

void VT::GraphicResourceManager::deleteVertexShaderReference(VertexShaderPool::HandleElementType handle)
{
	m_vertexShaders.removeElement(handle);
}

VT::VertexShaderResourceHandleReference VT::GraphicResourceManager::createVertexShader(const void* code, size_t codeSize)
{
	IVertexShader* shader = getGraphicDevice()->createVertexShader(code, codeSize);
	if (!shader)
	{
		return nullptr;
	}

	VertexShaderPool::NewElementInfo elementInfo = m_vertexShaders.addElementRaw();
	ManagedVertexShaderResourceHandle* shaderHandle
		= new (elementInfo.m_elementPtr) ManagedVertexShaderResourceHandle(shader, elementInfo.m_elementHandle.getKey());

	return shaderHandle;
}

VT::VertexShaderResourceHandleReference VT::GraphicResourceManager::getVertexShader(VertexShaderHandleID handle)
{
	return m_vertexShaders.getElementCast<VertexShaderResourceHandleReference>(handle);
}

bool VT::GraphicResourceManager::isValidVertexShader(VertexShaderHandleID handle) const
{
	return m_vertexShaders.isValid(handle);
}

VT::VertexShaderResourceHandleReference VT::GraphicResourceManager::loadVertexShader(const FileName& shaderPath)
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
			return createVertexShader(data, dataSize);
		}
	}

	return nullptr;
}

VT::VertexShaderResourceHandleReference VT::GraphicResourceManager::loadVertexShaderAsync(const FileName& shaderPath,
	OnLoadedVertexShaderCallback callback)
{
	VertexShaderPool::NewElementInfo elementInfo = m_vertexShaders.addElementRaw();
	ManagedVertexShaderResourceHandle* shaderHandle
		= new (elementInfo.m_elementPtr) ManagedVertexShaderResourceHandle(nullptr, elementInfo.m_elementHandle.getKey());
	VertexShaderResourceHandleReference shaderHandleReference = shaderHandle;

	IResourceSystem* resourceSystem = getResourceSystem();
	assert(resourceSystem);

	ResourceSystemConverterArgsReference args = resourceSystem->createResourceConverterArgs<ShaderResourceConverterArgs>(ShaderStageType::Vertex);
	resourceSystem->getResourceAsync(shaderPath, 
		[shaderHandleReference, onLoadedCallback = callback](ResourceDataReference resourceData)
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