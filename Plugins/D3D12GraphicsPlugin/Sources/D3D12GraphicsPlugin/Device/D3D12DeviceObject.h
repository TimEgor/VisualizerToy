#pragma once

#include "D3D12GraphicsPlugin/D3D12ComPtrDefines.h"

namespace VT_D3D12
{
	class D3D12DeviceObject
	{
	private:
		D3D12DeviceComPtr m_d3d12Device = nullptr;

	protected:
		D3D12DeviceComPtr getD3D12Device() const { return m_d3d12Device; }

	public:
		D3D12DeviceObject(D3D12DeviceComPtr d3d12Device)
			: m_d3d12Device(d3d12Device) {}
	};
}
