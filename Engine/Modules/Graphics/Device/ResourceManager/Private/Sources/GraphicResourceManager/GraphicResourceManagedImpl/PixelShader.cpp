#include "Common.h"

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