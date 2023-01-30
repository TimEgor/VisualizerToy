#include "Common.h"

#include "GraphicResourceCommon/IGraphicResourceDescriptor.h"

void VT::GraphicResourceManager::deleteShaderResourceViewInternal(IGraphicResourceDescriptor* descriptor)
{
	assert(descriptor);

	GraphicResourceDescriptorType descriptorType = descriptor->getType();

	if (descriptorType == GraphicResourceDescriptorType::CBV)
	{
		getGraphicDevice()->destroyBufferResourceDescriptor(descriptor);
	}
	else if (descriptorType == GraphicResourceDescriptorType::SRV)
	{
		getGraphicDevice()->destroyShaderResourceDescriptor(descriptor);
	}
	else
	{
		assert(false && "Invalid descriptor type.");
	}
}

void VT::GraphicResourceManager::deleteShaderResourceViewReference(ShaderResourceViewHandleID handleID)
{
	m_shaderResourceViews.removeElement(handleID);
}

VT::ShaderResourceViewReference VT::GraphicResourceManager::createShaderResourceDescriptor(GraphicResourceReference resource)
{
	IGraphicResource* deviceResource = resource ? resource->getResource() : nullptr;

	if (deviceResource)
	{
		IGraphicResourceDescriptor* deviceDescriptor = getGraphicDevice()->createShaderResourceDescriptor(deviceResource);

		if (deviceDescriptor)
		{
			ShaderResourceViewPool::NewElementInfo newDescriptorInfo = m_shaderResourceViews.addElementRaw();
			ManagedShaderGraphicResourceViewHandle* newDescriptorHandle =
				new (newDescriptorInfo.m_elementPtr) ManagedShaderGraphicResourceViewHandle(
					deviceDescriptor, newDescriptorInfo.m_elementHandle.getKey(),
					resource->getID(), deviceResource->getType()
				);

			return newDescriptorHandle;
		}
	}

	return nullptr;
}

VT::ShaderResourceViewReference VT::GraphicResourceManager::createBufferResourceDescriptor(GPUBufferReference buffer)
{
	IGPUBuffer* deviceBuffer = buffer ? buffer->getTypedResource() : nullptr;

	if (deviceBuffer)
	{
		IGraphicResourceDescriptor* deviceDescriptor = getGraphicDevice()->createBufferResourceDescriptor(deviceBuffer);

		if (deviceDescriptor)
		{
			ShaderResourceViewPool::NewElementInfo newDescriptorInfo = m_shaderResourceViews.addElementRaw();
			ManagedShaderGraphicResourceViewHandle* newDescriptorHandle =
				new (newDescriptorInfo.m_elementPtr) ManagedShaderGraphicResourceViewHandle(
					deviceDescriptor, newDescriptorInfo.m_elementHandle.getKey(),
					buffer->getID(), deviceBuffer->getType()
				);

			return newDescriptorHandle;
		}
	}

	return nullptr;
}