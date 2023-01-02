#pragma once

#include "InputLayout/IInputLayout.h"
#include "VulkanGraphicsPlugin/VulkanCore.h"

namespace VT_VK
{
	VkVertexInputRate converteInputLayoutBindingVTtoVK(const VT::InputLayoutBindingType bindingType)
	{
		switch (bindingType)
		{
		case VT::InputLayoutBindingType::VERTEX_BINDING:
			return VK_VERTEX_INPUT_RATE_VERTEX;
		case VT::InputLayoutBindingType::INSTANCE_BINDING:
			return VK_VERTEX_INPUT_RATE_INSTANCE;
		default:
			return VK_VERTEX_INPUT_RATE_MAX_ENUM;
		}
	}

	VkFormat convertInputLayoutFormatVTtoVK(VT::InputLayoutElementType type, uint32_t componentsNum)
	{
		switch (type)
		{
		case VT::InputLayoutElementType::FLOAT16:
		{
			switch (componentsNum)
			{
			case 1:
				return VK_FORMAT_R16_SFLOAT;
			case 2:
				return VK_FORMAT_R16G16_SFLOAT;
			case 3:
				return VK_FORMAT_R16G16B16_SFLOAT;
			case 4:
				return VK_FORMAT_R16G16B16A16_SFLOAT;
			default:
				return VK_FORMAT_UNDEFINED;
			}
		}

		case VT::InputLayoutElementType::FLOAT32:
		{
			switch (componentsNum)
			{
			case 1:
				return VK_FORMAT_R32_SFLOAT;
			case 2:
				return VK_FORMAT_R32G32_SFLOAT;
			case 3:
				return VK_FORMAT_R32G32B32_SFLOAT;
			case 4:
				return VK_FORMAT_R32G32B32A32_SFLOAT;
			default:
				return VK_FORMAT_UNDEFINED;
			}
		}

		case VT::InputLayoutElementType::INT32:
		{
			switch (componentsNum)
			{
			case 1:
				return VK_FORMAT_R32_SINT;
			case 2:
				return VK_FORMAT_R32G32_SINT;
			case 3:
				return VK_FORMAT_R32G32B32_SINT;
			case 4:
				return VK_FORMAT_R32G32B32A32_SINT;
			default:
				return VK_FORMAT_UNDEFINED;
			}
		}

		case VT::InputLayoutElementType::UINT32:
		{
			switch (componentsNum)
			{
			case 1:
				return VK_FORMAT_R32_UINT;
			case 2:
				return VK_FORMAT_R32G32_UINT;
			case 3:
				return VK_FORMAT_R32G32B32_UINT;
			case 4:
				return VK_FORMAT_R32G32B32A32_UINT;
			default:
				return VK_FORMAT_UNDEFINED;
			}
		}

		case VT::InputLayoutElementType::INT16:
		{
			switch (componentsNum)
			{
			case 1:
				return VK_FORMAT_R16_SINT;
			case 2:
				return VK_FORMAT_R16G16_SINT;
			case 3:
				return VK_FORMAT_R16G16B16_SINT;
			case 4:
				return VK_FORMAT_R16G16B16A16_SINT;
			default:
				return VK_FORMAT_UNDEFINED;
			}
		}

		case VT::InputLayoutElementType::UINT16:
		{
			switch (componentsNum)
			{
			case 1:
				return VK_FORMAT_R16_UINT;
			case 2:
				return VK_FORMAT_R16G16_UINT;
			case 3:
				return VK_FORMAT_R16G16B16_UINT;
			case 4:
				return VK_FORMAT_R16G16B16A16_UINT;
			default:
				return VK_FORMAT_UNDEFINED;
			}
		}

		case VT::InputLayoutElementType::INT8:
		{
			switch (componentsNum)
			{
			case 1:
				return VK_FORMAT_R8_SINT;
			case 2:
				return VK_FORMAT_R8G8_SINT;
			case 3:
				return VK_FORMAT_R8G8B8_SINT;
			case 4:
				return VK_FORMAT_R8G8B8A8_SINT;
			default:
				return VK_FORMAT_UNDEFINED;
			}
		}

		case VT::InputLayoutElementType::UINT8:
		{
			switch (componentsNum)
			{
			case 1:
				return VK_FORMAT_R8_UINT;
			case 2:
				return VK_FORMAT_R8G8_UINT;
			case 3:
				return VK_FORMAT_R8G8B8_UINT;
			case 4:
				return VK_FORMAT_R8G8B8A8_UINT;
			default:
				return VK_FORMAT_UNDEFINED;
			}
		}

		default:
			return VK_FORMAT_UNDEFINED;
		}
	}
}
