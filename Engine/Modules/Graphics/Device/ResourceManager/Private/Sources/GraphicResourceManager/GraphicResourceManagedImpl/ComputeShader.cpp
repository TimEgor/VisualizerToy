#include "Common.h"

void VT::GraphicResourceManager::deleteComputeShaderInternal(IComputeShader* shader)
{
	assert(shader);
	getGraphicDevice()->destroyComputeShader(shader);
}

void VT::GraphicResourceManager::deleteComputeShaderReference(ComputeShaderHandleID handleID)
{
	m_computeShaders.removeElement(handleID);
}

VT::ComputeShaderReference VT::GraphicResourceManager::createComputeShader(const void* code, size_t codeSize)
{
	IComputeShader* shader = getGraphicDevice()->createComputeShader(code, codeSize);
	if (!shader)
	{
		return nullptr;
	}

	ComputeShaderPool::NewElementInfo elementInfo = m_computeShaders.addElementRaw();
	ManagedComputeShaderGraphicObjectHandle* shaderHandle
		= new (elementInfo.m_elementPtr) ManagedComputeShaderGraphicObjectHandle(shader, elementInfo.m_elementHandle.getKey());

	return shaderHandle;
}

VT::ComputeShaderReference VT::GraphicResourceManager::getComputeShader(ComputeShaderHandleID handle)
{
	return m_computeShaders.getElementCast<ComputeShaderReference>(handle);
}

bool VT::GraphicResourceManager::isValidComputeShader(ComputeShaderHandleID handle) const
{
	return m_computeShaders.isValid(handle);
}