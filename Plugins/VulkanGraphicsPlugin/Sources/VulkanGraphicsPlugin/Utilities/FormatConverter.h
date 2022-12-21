#pragma once

#include "GraphicResourceCommon/Format.h"
#include "GraphicPipeline/IRenderPass.h"
#include "VulkanGraphicsPlugin/VulkanCore.h"

namespace VT_VK
{
	VkFormat convertFormat_VT_to_VK(const VT::Format format)
	{
		switch (format)
		{

		case VT::Format::R8_UNORM:
			return VK_FORMAT_R8_UNORM;
		case VT::Format::R8_SNORM:
			return VK_FORMAT_R8_SNORM;
		case VT::Format::R8_UINT:
			return VK_FORMAT_R8_UINT;
		case VT::Format::R8_SINT:
			return VK_FORMAT_R8_SINT;

		case VT::Format::R8G8_UNORM:
			return VK_FORMAT_R8G8_UNORM;
		case VT::Format::R8G8_SNORM:
			return VK_FORMAT_R8G8_SNORM;
		case VT::Format::R8G8_UINT:
			return VK_FORMAT_R8G8_UINT;
		case VT::Format::R8G8_SINT:
			return VK_FORMAT_R8G8_SINT;

		case VT::Format::R8G8B8_UNORM:
			return VK_FORMAT_R8G8B8_UNORM;
		case VT::Format::R8G8B8_SNORM:
			return VK_FORMAT_R8G8B8_SNORM;
		case VT::Format::R8G8B8_UINT:
			return VK_FORMAT_R8G8B8_UINT;
		case VT::Format::R8G8B8_SINT:
			return VK_FORMAT_R8G8B8_SINT;
		case VT::Format::B8G8R8_UNORM:
			return VK_FORMAT_B8G8R8_UNORM;
		case VT::Format::B8G8R8_SNORM:
			return VK_FORMAT_B8G8R8_SNORM;
		case VT::Format::B8G8R8_UINT:
			return VK_FORMAT_B8G8R8_UINT;
		case VT::Format::B8G8R8_SINT:
			return VK_FORMAT_B8G8R8_SINT;


		case VT::Format::R8G8B8A8_UNORM:
			return VK_FORMAT_R8G8B8A8_UNORM;
		case VT::Format::R8G8B8A8_SNORM:
			return VK_FORMAT_R8G8B8A8_SNORM;
		case VT::Format::R8G8B8A8_UINT:
			return VK_FORMAT_R8G8B8A8_UINT;
		case VT::Format::R8G8B8A8_SINT:
			return VK_FORMAT_R8G8B8A8_SINT;
		case VT::Format::B8G8R8A8_UNORM:
			return VK_FORMAT_B8G8R8A8_UNORM;
		case VT::Format::B8G8R8A8_SNORM:
			return VK_FORMAT_B8G8R8A8_SNORM;
		case VT::Format::B8G8R8A8_UINT:
			return VK_FORMAT_B8G8R8A8_UINT;
		case VT::Format::B8G8R8A8_SINT:
			return VK_FORMAT_B8G8R8A8_SINT;

		case VT::Format::R16_UNORM:
			return VK_FORMAT_R16_UNORM;
		case VT::Format::R16_SNORM:
			return VK_FORMAT_R16_SNORM;
		case VT::Format::R16_UINT:
			return VK_FORMAT_R16_UINT;
		case VT::Format::R16_SINT:
			return VK_FORMAT_R16_SINT;
		case VT::Format::R16_SFLOAT:
			return VK_FORMAT_R16_SFLOAT;

		case VT::Format::R16G16_UNORM:
			return VK_FORMAT_R16G16_UNORM;
		case VT::Format::R16G16_SNORM:
			return VK_FORMAT_R16G16_SNORM;
		case VT::Format::R16G16_UINT:
			return VK_FORMAT_R16G16_UINT;
		case VT::Format::R16G16_SINT:
			return VK_FORMAT_R16G16_SINT;
		case VT::Format::R16G16_SFLOAT:
			return VK_FORMAT_R16G16_SFLOAT;

		case VT::Format::R16G16B16_UNORM:
			return VK_FORMAT_R16G16B16_UNORM;
		case VT::Format::R16G16B16_SNORM:
			return VK_FORMAT_R16G16B16_SNORM;
		case VT::Format::R16G16B16_UINT:
			return VK_FORMAT_R16G16B16_UINT;
		case VT::Format::R16G16B16_SINT:
			return VK_FORMAT_R16G16B16_SINT;
		case VT::Format::R16G16B16_SFLOAT:
			return VK_FORMAT_R16G16B16_SFLOAT;

		case VT::Format::R16G16B16A16_UNORM:
			return VK_FORMAT_R16G16B16A16_UNORM;
		case VT::Format::R16G16B16A16_SNORM:
			return VK_FORMAT_R16G16B16A16_SNORM;
		case VT::Format::R16G16B16A16_UINT:
			return VK_FORMAT_R16G16B16A16_UINT;
		case VT::Format::R16G16B16A16_SINT:
			return VK_FORMAT_R16G16B16A16_SINT;
		case VT::Format::R16G16B16A16_SFLOAT:
			return VK_FORMAT_R16G16B16A16_SFLOAT;

		case VT::Format::R32_UINT:
			return VK_FORMAT_R32_UINT;
		case VT::Format::R32_SINT:
			return VK_FORMAT_R32_SINT;
		case VT::Format::R32_SFLOAT:
			return VK_FORMAT_R32_SFLOAT;

		case VT::Format::R32G32_UINT:
			return VK_FORMAT_R32G32_UINT;
		case VT::Format::R32G32_SINT:
			return VK_FORMAT_R32G32_SINT;
		case VT::Format::R32G32_SFLOAT:
			return VK_FORMAT_R32G32_SFLOAT;

		case VT::Format::R32G32B32_UINT:
			return VK_FORMAT_R32G32B32_UINT;
		case VT::Format::R32G32B32_SINT:
			return VK_FORMAT_R32G32B32_SINT;
		case VT::Format::R32G32B32_SFLOAT:
			return VK_FORMAT_R32G32B32_SFLOAT;

		case VT::Format::R32G32B32A32_UINT:
			return VK_FORMAT_R32G32B32A32_UINT;
		case VT::Format::R32G32B32A32_SINT:
			return VK_FORMAT_R32G32B32A32_SINT;
		case VT::Format::R32G32B32A32_SFLOAT:
			return VK_FORMAT_R32G32B32A32_SFLOAT;

		case VT::Format::R64_UINT:
			return VK_FORMAT_R64_UINT;
		case VT::Format::R64_SINT:
			return VK_FORMAT_R64_SINT;
		case VT::Format::R64_SFLOAT:
			return VK_FORMAT_R64_SFLOAT;

		case VT::Format::R64G64_UINT:
			return VK_FORMAT_R64G64_UINT;
		case VT::Format::R64G64_SINT:
			return VK_FORMAT_R64G64_SINT;
		case VT::Format::R64G64_SFLOAT:
			return VK_FORMAT_R64G64_SFLOAT;

		case VT::Format::R64G64B64_UINT:
			return VK_FORMAT_R64G64B64_UINT;
		case VT::Format::R64G64B64_SINT:
			return VK_FORMAT_R64G64B64_SINT;
		case VT::Format::R64G64B64_SFLOAT:
			return VK_FORMAT_R64G64B64_SFLOAT;

		case VT::Format::R64G64B64A64_UINT:
			return VK_FORMAT_R64G64B64A64_UINT;
		case VT::Format::R64G64B64A64_SINT:
			return VK_FORMAT_R64G64B64A64_SINT;
		case VT::Format::R64G64B64A64_SFLOAT:
			return VK_FORMAT_R64G64B64A64_SFLOAT;

		case VT::Format::D16_UNORM_S8_UINT:
			return VK_FORMAT_D16_UNORM_S8_UINT;
		case VT::Format::D24_UNORM_S8_UINT:
			return VK_FORMAT_D24_UNORM_S8_UINT;
		case VT::Format::D32_SFLOAT_S8_UINT:
			return VK_FORMAT_D32_SFLOAT_S8_UINT;

		case VT::Format::UNDEFINED:
		default:
			return VK_FORMAT_UNDEFINED;
		}
	}

	VkAttachmentLoadOp convertLoadingOperation_VT_to_VK(const VT::RenderPassAttachmentLoadingOperation operation)
	{
		switch (operation)
		{
		case VT::RenderPassAttachmentLoadingOperation::LOADING:
			return VK_ATTACHMENT_LOAD_OP_LOAD;
		case VT::RenderPassAttachmentLoadingOperation::CLEARING:
			return VK_ATTACHMENT_LOAD_OP_CLEAR;
		case VT::RenderPassAttachmentLoadingOperation::DONT_CARE:
		default:
			return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		}
	}

	VkAttachmentStoreOp convertStoringOperation_VT_to_VK(const VT::RenderPassAttachmentStoringOperation operation)
	{
		switch (operation)
		{
		case VT::RenderPassAttachmentStoringOperation::STORING:
			return VK_ATTACHMENT_STORE_OP_STORE;
		case VT::RenderPassAttachmentStoringOperation::DONT_CARE:
		default:
			return VK_ATTACHMENT_STORE_OP_DONT_CARE;
		}
	}

	VkImageLayout convertResourceLayout_VT_to_VK(const VT::ResourceLayout layout)
	{
		switch (layout) {
		case VT::ResourceLayout::COLOR:
			return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		case VT::ResourceLayout::PRESENT:
			return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		case VT::ResourceLayout::TRANSFER:
			return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		case VT::ResourceLayout::UNDEFINED:
		default:
			return VK_IMAGE_LAYOUT_UNDEFINED;
		}
	}
}