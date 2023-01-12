#include "D3D12GraphicsCommandList.h"

void VT_D3D12::D3D12GraphicsCommandList::close()
{
	m_d3d12CommandList->Close();
}

void VT_D3D12::D3D12GraphicsCommandList::reset()
{
	m_d3d12CommandAllocator->Reset();
	m_d3d12CommandList->Reset(m_d3d12CommandAllocator.Get(), nullptr);
}
