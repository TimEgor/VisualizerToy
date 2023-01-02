#include "Common.h"

void VT::GraphicResourceManager::deleteGPUBufferInternal(IGPUBuffer* buffer)
{
	getGraphicDevice()->destroyBuffer(buffer);
}

void VT::GraphicResourceManager::deleteGPUBufferReference(GPUBufferHandleID handleID)
{
	m_buffers.removeElement(handleID);
}

VT::GPUBufferReference VT::GraphicResourceManager::createGPUBuffer(const GPUBufferDesc& desc)
{
	IGPUBuffer* buffer = getGraphicDevice()->createBuffer(desc);
	if (!buffer)
	{
		return nullptr;
	}

	GPUBufferPool::NewElementInfo elementInfo = m_buffers.addElementRaw();
	ManagedGPUBufferResourceHandle* bufferHandle
		= new (elementInfo.m_elementPtr) ManagedGPUBufferResourceHandle(buffer, elementInfo.m_elementHandle.getKey());

	return bufferHandle;
}

VT::GPUBufferReference VT::GraphicResourceManager::getGPUBuffer(GPUBufferHandleID handle)
{
	return m_buffers.getElementCast<GPUBufferReference>(handle);
}

bool VT::GraphicResourceManager::isValidGPUBuffer(GPUBufferHandleID handle)
{
	return m_buffers.isValid(handle);
}