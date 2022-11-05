#pragma once

#include "SwapChain/ISwapChain.h"
#include "VulkanGraphicsPlugin/VulkanCore.h"

namespace VT_VK
{
	VkPresentModeKHR convertPresentMode_VT_to_VK(const VT::SwapChainPresentMode mode)
	{
		switch (mode)
		{
		case VT::SwapChainPresentMode::FIFO:
			return VK_PRESENT_MODE_FIFO_KHR;
		case VT::SwapChainPresentMode::FIFO_RELAX:
			return VK_PRESENT_MODE_FIFO_RELAXED_KHR;
		case VT::SwapChainPresentMode::MAILBOX:
			return VK_PRESENT_MODE_MAILBOX_KHR;

		case VT::SwapChainPresentMode::IMMIDIATE:
		default:
			return VK_PRESENT_MODE_IMMEDIATE_KHR;
		}
	}
}