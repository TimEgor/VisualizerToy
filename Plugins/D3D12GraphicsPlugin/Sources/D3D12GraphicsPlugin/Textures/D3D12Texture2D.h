#pragma once

#include "D3D12GraphicsPlugin/D3D12ComPtrDefines.h"
#include "D3D12GraphicsPlugin/Resource/D3D12Resource.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedTextureObjects.h"

namespace VT_D3D12
{
	class D3D12Texture2D final : public VT::ManagedGraphicDevice::ManagedTexture2DBase, public D3D12ResourceBase
	{
	public:
		D3D12Texture2D(const VT::Texture2DDesc& desc, D3D12ResourceComPtr d3d12Resource)
			: ManagedTexture2DBase(desc), D3D12ResourceBase(d3d12Resource, nullptr) {}
		D3D12Texture2D(const VT::Texture2DDesc& desc,
			D3D12ResourceComPtr d3d12Resource, D3D12MemAllocationComPtr d3d12MemAllocation)
			: ManagedTexture2DBase(desc), D3D12ResourceBase(d3d12Resource, d3d12MemAllocation) {}

		virtual void* getNativeHandle() const override { return getD3D12Resource().Get(); }
	};
}