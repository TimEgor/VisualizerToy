#pragma once

#include "InputLayout/InputLayout.h"
#include "D3D12GraphicsPlugin/D3D12Core.h"

namespace VT_D3D12
{
	inline D3D12_INPUT_CLASSIFICATION convertInputLayoutBindingTypeVTtoD3D12(const VT::InputLayoutBindingType bindingType)
	{
		switch (bindingType)
		{
		case VT::InputLayoutBindingType::VERTEX_BINDING:
			return D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		case VT::InputLayoutBindingType::INSTANCE_BINDING:
			return D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;
		default:
			return static_cast<D3D12_INPUT_CLASSIFICATION>(-1);
		}
	}

	inline DXGI_FORMAT convertInputLayoutFormatVTtoDXGI(VT::InputLayoutElementType type, uint32_t componentsNum)
	{
		switch (type)
		{
		case VT::InputLayoutElementType::FLOAT16:
		{
			switch (componentsNum)
			{
			case 1:
				return DXGI_FORMAT_R16_FLOAT;
			case 2:
				return DXGI_FORMAT_R16G16_FLOAT;
			case 4:
				return DXGI_FORMAT_R16G16B16A16_FLOAT;
			default:
				return DXGI_FORMAT_UNKNOWN;
			}
		}

		case VT::InputLayoutElementType::FLOAT32:
		{
			switch (componentsNum)
			{
			case 1:
				return DXGI_FORMAT_R32_FLOAT;
			case 2:
				return DXGI_FORMAT_R32G32_FLOAT;
			case 3:
				return DXGI_FORMAT_R32G32B32_FLOAT;
			case 4:
				return DXGI_FORMAT_R32G32B32A32_FLOAT;
			default:
				return DXGI_FORMAT_UNKNOWN;
			}
		}

		case VT::InputLayoutElementType::INT32:
		{
			switch (componentsNum)
			{
			case 1:
				return DXGI_FORMAT_R32_SINT;
			case 2:
				return DXGI_FORMAT_R32G32_SINT;
			case 3:
				return DXGI_FORMAT_R32G32B32_SINT;
			case 4:
				return DXGI_FORMAT_R32G32B32A32_SINT;
			default:
				return DXGI_FORMAT_UNKNOWN;
			}
		}

		case VT::InputLayoutElementType::UINT32:
		{
			switch (componentsNum)
			{
			case 1:
				return DXGI_FORMAT_R32_UINT;
			case 2:
				return DXGI_FORMAT_R32G32_UINT;
			case 3:
				return DXGI_FORMAT_R32G32B32_UINT;
			case 4:
				return DXGI_FORMAT_R32G32B32A32_UINT;
			default:
				return DXGI_FORMAT_UNKNOWN;
			}
		}

		case VT::InputLayoutElementType::INT16:
		{
			switch (componentsNum)
			{
			case 1:
				return DXGI_FORMAT_R16_SINT;
			case 2:
				return DXGI_FORMAT_R16G16_SINT;
			case 4:
				return DXGI_FORMAT_R16G16B16A16_SINT;
			default:
				return DXGI_FORMAT_UNKNOWN;
			}
		}

		case VT::InputLayoutElementType::UINT16:
		{
			switch (componentsNum)
			{
			case 1:
				return DXGI_FORMAT_R16_UINT;
			case 2:
				return DXGI_FORMAT_R16G16_UINT;
			case 4:
				return DXGI_FORMAT_R16G16B16A16_UINT;
			default:
				return DXGI_FORMAT_UNKNOWN;
			}
		}

		case VT::InputLayoutElementType::INT8:
		{
			switch (componentsNum)
			{
			case 1:
				return DXGI_FORMAT_R8_SINT;
			case 2:
				return DXGI_FORMAT_R8G8_SINT;
			case 4:
				return DXGI_FORMAT_R8G8B8A8_SINT;
			default:
				return DXGI_FORMAT_UNKNOWN;
			}
		}

		case VT::InputLayoutElementType::UINT8:
		{
			switch (componentsNum)
			{
			case 1:
				return DXGI_FORMAT_R8_UINT;
			case 2:
				return DXGI_FORMAT_R8G8_UINT;
			case 4:
				return DXGI_FORMAT_R8G8B8A8_UINT;
			default:
				return DXGI_FORMAT_UNKNOWN;
			}
		}

		default:
			return DXGI_FORMAT_UNKNOWN;
		}
	}

	inline void convertInputLayoutVTtoD3D12(const VT::InputLayoutDesc& desc, std::vector<D3D12_INPUT_ELEMENT_DESC>& d3d12Descs)
	{
		const size_t elementsCount = desc.m_elements.size();
		d3d12Descs.resize(elementsCount);

		for (size_t elementIndex = 0; elementIndex < elementsCount; ++elementIndex)
		{
			const VT::InputLayoutElementDesc& elementDesc = desc.m_elements[elementIndex];
			const VT::InputLayoutBindingDesc& elementBindingDesc = desc.m_bindings[elementDesc.m_slot];

			D3D12_INPUT_ELEMENT_DESC& d3D12ElementDesc = d3d12Descs[elementIndex];
			d3D12ElementDesc.SemanticName = elementDesc.m_semanticName.data();
			d3D12ElementDesc.SemanticIndex = elementDesc.m_index;
			d3D12ElementDesc.Format = convertInputLayoutFormatVTtoDXGI(elementDesc.m_type, elementDesc.m_componentsCount);
			d3D12ElementDesc.InputSlot = elementDesc.m_slot;
			d3D12ElementDesc.AlignedByteOffset = elementDesc.m_offset;
			d3D12ElementDesc.InputSlotClass = convertInputLayoutBindingTypeVTtoD3D12(elementBindingDesc.m_type);
			d3D12ElementDesc.InstanceDataStepRate = 0;
		}
	}
}
