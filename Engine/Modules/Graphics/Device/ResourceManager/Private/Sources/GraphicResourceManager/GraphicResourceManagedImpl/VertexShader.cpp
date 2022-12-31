#include "Common.h"

void VT::GraphicResourceManager::deleteVertexShaderInternal(IVertexShader* shader)
{
	getGraphicDevice()->destroyVertexShader(shader);
}

void VT::GraphicResourceManager::deleteVertexShaderReference(VertexShaderHandleID handleID)
{
	m_vertexShaders.removeElement(handleID);
}

VT::VertexShaderReference VT::GraphicResourceManager::createVertexShader(const void* code, size_t codeSize)
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

VT::VertexShaderReference VT::GraphicResourceManager::getVertexShader(VertexShaderHandleID handle)
{
	return m_vertexShaders.getElementCast<VertexShaderReference>(handle);
}

bool VT::GraphicResourceManager::isValidVertexShader(VertexShaderHandleID handle) const
{
	return m_vertexShaders.isValid(handle);
}