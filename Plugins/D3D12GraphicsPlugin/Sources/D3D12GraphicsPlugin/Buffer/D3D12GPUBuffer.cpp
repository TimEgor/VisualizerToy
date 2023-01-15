#include "D3D12GPUBuffer.h"

void VT_D3D12::D3D12GPUBuffer::mapData(void** data)
{
	m_d3d12Resource->Map(0, nullptr, data);
}

void VT_D3D12::D3D12GPUBuffer::unmapData()
{
	m_d3d12Resource->Unmap(0, nullptr);
}
