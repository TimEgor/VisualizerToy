#include "VulkanRenderContext.h"

#include "Core/UtilitiesMacros.h"

#include "VulkanGraphicsPlugin/Commands/VulkanCommandPool.h"
#include "VulkanGraphicsPlugin/PipelineState/VulkanPipelineState.h"
#include "VulkanGraphicsPlugin/Textures/VulkanTexture2D.h"
#include "VulkanGraphicsPlugin/Buffer/VulkanGPUBuffer.h"

bool VT_VK::VulkanRenderContext::init(VT::ICommandList* commandList)
{
	m_commandList = reinterpret_cast<VulkanCommandList*>(commandList);
	VT_CHECK_INITIALIZATION(m_commandList);

	return true;
}

void VT_VK::VulkanRenderContext::release()
{
}

VT::ICommandList* VT_VK::VulkanRenderContext::getCommandList()
{
	return m_commandList;
}

void VT_VK::VulkanRenderContext::begin()
{
	vkResetCommandBuffer(m_commandList->getVkCommandBuffer(), 0);

	//
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	checkVkResultAssert(vkBeginCommandBuffer(m_commandList->getVkCommandBuffer(), &beginInfo));
}

void VT_VK::VulkanRenderContext::end()
{
	vkEndCommandBuffer(m_commandList->getVkCommandBuffer());
}

void VT_VK::VulkanRenderContext::beginRendering(const VT::RenderContextBeginInfo& info)
{
	assert(info.m_targets.size());
	const VT::Texture2DDesc& targetDesc = info.m_targets[0].m_texture->getDesc();

	VkRect2D renderArea = VkRect2D{ VkOffset2D{}, VkExtent2D{ targetDesc.m_width, targetDesc.m_height } };

	VkClearValue targetClearVal = { { 0.5f, 0.5f, 0.5f, 1.0f } };

	std::vector<VkRenderingAttachmentInfoKHR> attachments;
	attachments.reserve(info.m_targets.size());

	for (size_t i = 0; i < info.m_targets.size(); ++i)
	{
		VulkanTexture2DView* vulkanTargetView = reinterpret_cast<VulkanTexture2DView*>(info.m_targets[i].m_view);

		const VT::Texture2DDesc& targetTextureDesc = info.m_targets[i].m_texture->getDesc();
		assert(renderArea.extent.width == targetTextureDesc.m_width && renderArea.extent.height == targetTextureDesc.m_height);

		VkRenderingAttachmentInfoKHR renderingAttachmentInfo{};
		renderingAttachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
		renderingAttachmentInfo.imageView = vulkanTargetView->getVkImageView();
		renderingAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;
		renderingAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		renderingAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		renderingAttachmentInfo.clearValue = targetClearVal;

		attachments.push_back(renderingAttachmentInfo);
	}


	VkRenderingInfoKHR renderInfo{};
	renderInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
	renderInfo.renderArea = renderArea;
	renderInfo.layerCount = 1;
	renderInfo.colorAttachmentCount = attachments.size();
	renderInfo.pColorAttachments = attachments.data();

	vkCmdBeginRenderingKHR(m_commandList->getVkCommandBuffer(), &renderInfo);
}

void VT_VK::VulkanRenderContext::endRendering()
{
	vkCmdEndRenderingKHR(m_commandList->getVkCommandBuffer());
}

void VT_VK::VulkanRenderContext::prepareTextureForRendering(VT::ITexture2D* texture)
{
	VkImageMemoryBarrier textureBarrier{};
	textureBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	textureBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	textureBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	textureBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	textureBarrier.image = reinterpret_cast<VulkanTexture2D*>(texture)->getVkImage();
	textureBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	textureBarrier.subresourceRange.baseMipLevel = 0;
	textureBarrier.subresourceRange.levelCount = 1;
	textureBarrier.subresourceRange.baseArrayLayer = 0;
	textureBarrier.subresourceRange.layerCount = 1;

	vkCmdPipelineBarrier(
		m_commandList->getVkCommandBuffer(),
		VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,  // srcStageMask
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // dstStageMask
		0,
		0,
		nullptr,
		0,
		nullptr,
		1, // imageMemoryBarrierCount
		&textureBarrier // pImageMemoryBarriers
	);
}

void VT_VK::VulkanRenderContext::prepareTextureForPresenting(VT::ITexture2D* texture)
{
	VkImageMemoryBarrier textureBarrier{};
	textureBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	textureBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	textureBarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	textureBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	textureBarrier.image = reinterpret_cast<VulkanTexture2D*>(texture)->getVkImage();
	textureBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	textureBarrier.subresourceRange.baseMipLevel = 0;
	textureBarrier.subresourceRange.levelCount = 1;
	textureBarrier.subresourceRange.baseArrayLayer = 0;
	textureBarrier.subresourceRange.layerCount = 1;

	vkCmdPipelineBarrier(
		m_commandList->getVkCommandBuffer(),
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,  // srcStageMask
		VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, // dstStageMask
		0,
		0,
		nullptr,
		0,
		nullptr,
		1, // imageMemoryBarrierCount
		&textureBarrier // pImageMemoryBarriers
	);
}

void VT_VK::VulkanRenderContext::setPipelineState(VT::IPipelineState* pipelineState)
{
	VulkanPipelineState* vulkanPipelineState = reinterpret_cast<VulkanPipelineState*>(pipelineState);
	vkCmdBindPipeline(m_commandList->getVkCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipelineState->getVkPipeline());
}

void VT_VK::VulkanRenderContext::setVertexBuffer(VT::IGPUBuffer* buffer)
{
	VulkanGPUBuffer* vulkanBuffer = reinterpret_cast<VulkanGPUBuffer*>(buffer);

	VkDeviceSize offset = 0;
	VkBuffer vkBuffer = vulkanBuffer->getVkBuffer();

	vkCmdBindVertexBuffers(m_commandList->getVkCommandBuffer(), 0, 1, &vkBuffer, &offset);
}

void VT_VK::VulkanRenderContext::setVertexBuffers(uint32_t buffersCount, VT::IGPUBuffer* buffers)
{
	std::vector<VkBuffer> vkBuffers;
	std::vector<VkDeviceSize> vkOffsets;

	vkBuffers.reserve(buffersCount);
	vkOffsets.reserve(buffersCount);

	for (uint32_t i = 0; i < buffersCount; ++i)
	{
		VulkanGPUBuffer* vulkanBuffer = reinterpret_cast<VulkanGPUBuffer*>(&buffers[i]);

		vkBuffers.push_back(vulkanBuffer->getVkBuffer());
		vkOffsets.push_back(0);
	}

	vkCmdBindVertexBuffers(m_commandList->getVkCommandBuffer(), 0, buffersCount, vkBuffers.data(), vkOffsets.data());
}

void VT_VK::VulkanRenderContext::setIndexBuffer(VT::IGPUBuffer* buffer)
{
	VulkanGPUBuffer* vulkanBuffer = reinterpret_cast<VulkanGPUBuffer*>(buffer);

	VkDeviceSize offset = 0;
	VkBuffer vkBuffer = vulkanBuffer->getVkBuffer();

	vkCmdBindIndexBuffer(m_commandList->getVkCommandBuffer(), vkBuffer, offset, VkIndexType::VK_INDEX_TYPE_UINT16);
}

void VT_VK::VulkanRenderContext::draw(uint32_t vertCount)
{
	vkCmdDraw(m_commandList->getVkCommandBuffer(), vertCount, 1, 0, 0);
}

void VT_VK::VulkanRenderContext::drawIndexed(uint32_t indexCount)
{
	vkCmdDrawIndexed(m_commandList->getVkCommandBuffer(), indexCount, 1, 0, 0, 0);
}
