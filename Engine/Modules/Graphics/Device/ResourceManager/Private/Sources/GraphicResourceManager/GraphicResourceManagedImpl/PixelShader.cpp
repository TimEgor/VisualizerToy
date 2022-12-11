#include "GraphicResourceManager/CommonGraphicResourceManager.h"

#include "ShaderConverter/ShaderConverterArgs.h"

void VT::GraphicResourceManager::deletePixelShaderInternal(IPixelShader* shader)
{
	getGraphicDevice()->destroyPixelShader(shader);
}

void VT::GraphicResourceManager::deletePixelShaderReference(PixelShaderPool::HandleElementType handle)
{
	m_pixelShaders.removeElement(handle);
}

VT::PixelShaderResourceHandleReference VT::GraphicResourceManager::createPixelShader(const void* code, size_t codeSize)
{
	IPixelShader* shader = getGraphicDevice()->createPixelShader(code, codeSize);
	if (!shader)
	{
		return nullptr;
	}

	PixelShaderPool::NewElementInfo elementInfo = m_pixelShaders.addElementRaw();
	ManagedPixelShaderResourceHandle* shaderHandle
		= new (elementInfo.m_elementPtr) ManagedPixelShaderResourceHandle(shader, elementInfo.m_elementHandle.getKey());

	return shaderHandle;
}

VT::PixelShaderResourceHandleReference VT::GraphicResourceManager::getPixelShader(PixelShaderHandleID handle)
{
	return m_pixelShaders.getElementCast<PixelShaderResourceHandleReference>(handle);
}

bool VT::GraphicResourceManager::isValidPixelShader(PixelShaderHandleID handle) const
{
	return m_pixelShaders.isValid(handle);
}

VT::PixelShaderResourceHandleReference VT::GraphicResourceManager::loadPixelShader(const FileName& shaderPath)
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
			return createPixelShader(data, dataSize);
		}
	}

	return nullptr;
}

VT::PixelShaderResourceHandleReference VT::GraphicResourceManager::loadPixelShaderAsync(const FileName& shaderPath,
	OnLoadedPixelShaderCallback callback)
{
	PixelShaderPool::NewElementInfo elementInfo = m_pixelShaders.addElementRaw();
	ManagedPixelShaderResourceHandle* shaderHandle
		= new (elementInfo.m_elementPtr) ManagedPixelShaderResourceHandle(nullptr, elementInfo.m_elementHandle.getKey());
	PixelShaderResourceHandleReference shaderHandleReference = shaderHandle;

	IResourceSystem* resourceSystem = getResourceSystem();
	assert(resourceSystem);

	ResourceSystemConverterArgsReference args = resourceSystem->createResourceConverterArgs<ShaderResourceConverterArgs>(ShaderStageType::Pixel);
	resourceSystem->getResourceAsync(shaderPath,
		[shaderHandleReference, onLoadedCallback = callback](ResourceDataReference resourceData)
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