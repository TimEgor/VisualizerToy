#pragma once

#include "D3D12GraphicsPlugin/D3D12Core.h"
#include "GraphicResourceCommon/IGraphicResourceDescriptor.h"

namespace VT_D3D12
{
	D3D12_DESCRIPTOR_HEAP_TYPE convertDescriptorTypeVTtoD3D12(VT::GraphicResourceDescriptorType type)
	{
		switch (type)
		{
		case VT::GraphicResourceDescriptorType::RTV:
			return D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		case VT::GraphicResourceDescriptorType::DSV:
			return D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		case VT::GraphicResourceDescriptorType::SRV:
			return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

		case VT::GraphicResourceDescriptorType::UNDEFINED:
		default:
			return static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(-1);
		}
	}
}
