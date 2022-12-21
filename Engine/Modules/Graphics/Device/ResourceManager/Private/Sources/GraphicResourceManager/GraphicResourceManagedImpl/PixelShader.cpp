#include "Common.h"

#include "ShaderConverter/ShaderConverterArgs.h"

void VT::GraphicResourceManager::deletePixelShaderInternal(IPixelShader* shader)
{
	getGraphicDevice()->destroyPixelShader(shader);
}

void VT::GraphicResourceManager::deletePixelShaderReference(PixelShaderPool::Handle handle)
{
	m_pixelShaders.removeElement(handle);
}

VT::PixelShaderReference VT::GraphicResourceManager::createPixelShader(const void* code, size_t codeSize)
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

VT::PixelShaderReference VT::GraphicResourceManager::getPixelShader(PixelShaderHandleID handle)
{
	return m_pixelShaders.getElementCast<PixelShaderReference>(handle);
}

bool VT::GraphicResourceManager::isValidPixelShader(PixelShaderHandleID handle) const
{
	return m_pixelShaders.isValid(handle);
}