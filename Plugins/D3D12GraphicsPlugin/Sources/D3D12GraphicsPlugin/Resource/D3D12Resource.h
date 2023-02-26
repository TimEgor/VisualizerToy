#pragma once

#include "D3D12GraphicsPlugin/D3D12ComPtrDefines.h"

namespace VT_D3D12
{
	class D3D12ResourceBase
	{
	protected:
		D3D12ResourceComPtr m_d3d12Resource = nullptr;
		D3D12MemAllocationComPtr m_d3d12MemAllocation = nullptr;

	public:
		D3D12ResourceBase(D3D12ResourceComPtr d3d12Resource)
			: m_d3d12Resource(d3d12Resource) {}
		D3D12ResourceBase(D3D12ResourceComPtr d3d12Resource, D3D12MemAllocationComPtr d3d12MemAllocation)
			: m_d3d12Resource(d3d12Resource), m_d3d12MemAllocation(d3d12MemAllocation) {}
		virtual ~D3D12ResourceBase() = default;

		D3D12ResourceComPtr getD3D12Resource() const { return m_d3d12Resource; }
	};
}
