#pragma once

#include "D3D12GraphicsPlugin/Device/D3D12DeviceObject.h"
#include "D3D12GraphicsPlugin/Resource/D3D12Resource.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedBuffer.h"

namespace VT_D3D12
{
	class D3D12GPUBuffer final : public VT::ManagedGraphicDevice::ManagedGPUBufferBase, public D3D12DeviceObject, public D3D12ResourceBase
	{
	public:
		D3D12GPUBuffer(const VT::GPUBufferDesc& desc, D3D12DeviceObject d3d12Device, D3D12ResourceComPtr d3d12Resource)
			: ManagedGPUBufferBase(desc), D3D12DeviceObject(d3d12Device), D3D12ResourceBase(d3d12Resource) {}

		virtual void mapData(void** data) override;
		virtual void unmapData() override;

		virtual void* getNativeHandle() const override { return getD3D12Resource().Get(); }
	};
}
