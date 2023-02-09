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

void VT::GraphicResourceManager::deleteRenderTargetViewInternal(IGraphicResourceDescriptor* descriptor)
{
	assert(descriptor);
	getGraphicDevice()->destroyRenderTargetDescriptor(descriptor);
}

void VT::GraphicResourceManager::deleteRenderTargetViewReference(RenderTargetViewHandleID handleID)
{
	m_renderTargetViews.removeElement(handleID);
}

VT::RenderTargetViewReference VT::GraphicResourceManager::createRenderTargetDescriptor(TextureReference texture)
{
	ITexture* deviceTextures = texture ? texture->getTexture() : nullptr;

	if (deviceTextures)
	{
		IGraphicResourceDescriptor* deviceDescriptor = getGraphicDevice()->createRenderTargetDescriptor(deviceTextures);

		if (deviceDescriptor)
		{
			RenderTargetViewPool::NewElementInfo newDescriptorInfo = m_renderTargetViews.addElementRaw();
			ManagedRenderTargetGraphicResourceViewHandle* newDescriptorHandle =
				new (newDescriptorInfo.m_elementPtr) ManagedRenderTargetGraphicResourceViewHandle(
					deviceDescriptor, newDescriptorInfo.m_elementHandle.getKey(),
					texture->getID(), deviceTextures->getType()
				);

			return newDescriptorHandle;
		}
	}

	return nullptr;
}

VT::DepthStencilViewReference VT::GraphicResourceManager::createDepthStencilDescriptor(TextureReference texture)
{
	ITexture* deviceTextures = texture ? texture->getTexture() : nullptr;

	if (deviceTextures)
	{
		IGraphicResourceDescriptor* deviceDescriptor = getGraphicDevice()->createDepthStencilDescriptor(deviceTextures);

		if (deviceDescriptor)
		{
			RenderTargetViewPool::NewElementInfo newDescriptorInfo = m_renderTargetViews.addElementRaw();
			ManagedRenderTargetGraphicResourceViewHandle* newDescriptorHandle =
				new (newDescriptorInfo.m_elementPtr) ManagedRenderTargetGraphicResourceViewHandle(
					deviceDescriptor, newDescriptorInfo.m_elementHandle.getKey(),
					texture->getID(), deviceTextures->getType()
				);

			return newDescriptorHandle;
		}
	}

	return nullptr;
}
