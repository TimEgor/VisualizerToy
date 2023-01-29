#pragma once

#include "D3D12GraphicsPlugin/D3D12ComPtrDefines.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedCommandObjects.h"

namespace VT_D3D12
{
	class D3D12GraphicsCommandList final : public VT::ManagedGraphicDevice::ManagedCommandListBase
	{
	private:
		D3D12GraphicsCommandListComPtr m_d3d12CommandList;
		D3D12CommandAllocatorComPtr m_d3d12CommandAllocator;

	public:
		D3D12GraphicsCommandList(D3D12GraphicsCommandListComPtr d3d12CommandList, D3D12CommandAllocatorComPtr d3d12CommandAllocator)
			: m_d3d12CommandList(d3d12CommandList), m_d3d12CommandAllocator(d3d12CommandAllocator) {}

		virtual void close() override;
		virtual void reset() override;

		D3D12GraphicsCommandListComPtr getD3D12CommandList() const { return m_d3d12CommandList; }
		D3D12CommandAllocatorComPtr getD3D12CommandAllocator() const { return m_d3d12CommandAllocator; }

		virtual void* getNativeHandle() const override { return getD3D12CommandList().Get(); }
	};
}