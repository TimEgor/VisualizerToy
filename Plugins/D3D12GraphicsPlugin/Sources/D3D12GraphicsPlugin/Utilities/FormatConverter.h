#pragma once

#include "GraphicResourceCommon/Format.h"
#include "D3D12GraphicsPlugin/D3D12Core.h"

namespace VT_D3D12
{
	inline DXGI_FORMAT convertFormatVTtoD3D12(const VT::Format format)
	{
		switch (format)
		{
		case VT::Format::R8_UNORM:
			return DXGI_FORMAT_R8_UNORM;
		case VT::Format::R8_SNORM:
			return DXGI_FORMAT_R8_SNORM;
		case VT::Format::R8_UINT:
			return DXGI_FORMAT_R8_UINT;
		case VT::Format::R8_SINT:
			return DXGI_FORMAT_R8_SINT;

		case VT::Format::R8G8_UNORM:
			return DXGI_FORMAT_R8G8_UNORM;
		case VT::Format::R8G8_SNORM:
			return DXGI_FORMAT_R8G8_SNORM;
		case VT::Format::R8G8_UINT:
			return DXGI_FORMAT_R8G8_UINT;
		case VT::Format::R8G8_SINT:
			return DXGI_FORMAT_R8G8_SINT;

		case VT::Format::R8G8B8A8_UNORM:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		case VT::Format::R8G8B8A8_SNORM:
			return DXGI_FORMAT_R8G8B8A8_SNORM;
		case VT::Format::R8G8B8A8_UINT:
			return DXGI_FORMAT_R8G8B8A8_UINT;
		case VT::Format::R8G8B8A8_SINT:
			return DXGI_FORMAT_R8G8B8A8_SINT;

		case VT::Format::R16_UNORM:
			return DXGI_FORMAT_R16_UNORM;
		case VT::Format::R16_SNORM:
			return DXGI_FORMAT_R16_SNORM;
		case VT::Format::R16_UINT:
			return DXGI_FORMAT_R16_UINT;
		case VT::Format::R16_SINT:
			return DXGI_FORMAT_R16_SINT;
		case VT::Format::R16_SFLOAT:
			return DXGI_FORMAT_R16_FLOAT;

		case VT::Format::R16G16_UNORM:
			return DXGI_FORMAT_R16G16_UNORM;
		case VT::Format::R16G16_SNORM:
			return DXGI_FORMAT_R16G16_SNORM;
		case VT::Format::R16G16_UINT:
			return DXGI_FORMAT_R16G16_UINT;
		case VT::Format::R16G16_SINT:
			return DXGI_FORMAT_R16G16_SINT;
		case VT::Format::R16G16_SFLOAT:
			return DXGI_FORMAT_R16G16_FLOAT;

		case VT::Format::R16G16B16A16_UNORM:
			return DXGI_FORMAT_R16G16B16A16_UNORM;
		case VT::Format::R16G16B16A16_SNORM:
			return DXGI_FORMAT_R16G16B16A16_SNORM;
		case VT::Format::R16G16B16A16_UINT:
			return DXGI_FORMAT_R16G16B16A16_UINT;
		case VT::Format::R16G16B16A16_SINT:
			return DXGI_FORMAT_R16G16B16A16_SINT;
		case VT::Format::R16G16B16A16_SFLOAT:
			return DXGI_FORMAT_R16G16B16A16_FLOAT;

		case VT::Format::R32_UINT:
			return DXGI_FORMAT_R32_UINT;
		case VT::Format::R32_SINT:
			return DXGI_FORMAT_R32_SINT;
		case VT::Format::R32_SFLOAT:
			return DXGI_FORMAT_R32_FLOAT;

		case VT::Format::R32G32_UINT:
			return DXGI_FORMAT_R32G32_UINT;
		case VT::Format::R32G32_SINT:
			return DXGI_FORMAT_R32G32_SINT;
		case VT::Format::R32G32_SFLOAT:
			return DXGI_FORMAT_R32G32_FLOAT;

		case VT::Format::R32G32B32_UINT:
			return DXGI_FORMAT_R32G32B32_UINT;
		case VT::Format::R32G32B32_SINT:
			return DXGI_FORMAT_R32G32B32_SINT;
		case VT::Format::R32G32B32_SFLOAT:
			return DXGI_FORMAT_R32G32B32_FLOAT;

		case VT::Format::R32G32B32A32_UINT:
			return DXGI_FORMAT_R32G32B32A32_UINT;
		case VT::Format::R32G32B32A32_SINT:
			return DXGI_FORMAT_R32G32B32A32_SINT;
		case VT::Format::R32G32B32A32_SFLOAT:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		
		case VT::Format::D24_UNORM_S8_UINT:
			return DXGI_FORMAT_D24_UNORM_S8_UINT;
		case VT::Format::D32_SFLOAT_S8_UINT:
			return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;

		case VT::Format::UNDEFINED:
		default:
			return DXGI_FORMAT_UNKNOWN;
		}
	}
}