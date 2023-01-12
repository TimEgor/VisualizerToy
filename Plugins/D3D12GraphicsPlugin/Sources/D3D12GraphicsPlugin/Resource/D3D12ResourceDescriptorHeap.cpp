#include "D3D12ResourceDescriptorHeap.h"

#include <cassert>

VT_D3D12::D3D12ResourceDescriptorHeap::D3D12ResourceDescriptorHeap(const VT::GraphicResourceDescriptorHeapDesc& desc,
	D3D12DescriptorHeapComPtr d3d12DescriptorHeap, uint32_t descriptorSize)
	: GraphicResourceDescriptorHeapBase(desc), m_d3d12Heap(d3d12DescriptorHeap), m_descriptorSize(descriptorSize)
{
	m_baseCPUHeapHandle = m_d3d12Heap->GetCPUDescriptorHandleForHeapStart();
	if (desc.m_isShaderVisible)
	{
		m_baseGPUHeapHandle = m_d3d12Heap->GetGPUDescriptorHandleForHeapStart();
	}

	m_freeIndies.reserve(desc.m_elementsCount);

	VT::DescriptorBindingHeapOffsetType indexVal = desc.m_elementsCount - 1;
	for (uint32_t i = 0; i < desc.m_elementsCount; ++i)
	{
		m_freeIndies.push_back(indexVal);
		indexVal--;
	}
}

VT::DescriptorBindingHeapOffsetType VT_D3D12::D3D12ResourceDescriptorHeap::allocateDescriptor()
{
	assert(!m_freeIndies.empty());

	const VT::DescriptorBindingHeapOffsetType offset = m_freeIndies.back();
	m_freeIndies.pop_back();

	return offset;
}

void VT_D3D12::D3D12ResourceDescriptorHeap::deallocateDescriptor(VT::DescriptorBindingHeapOffsetType offset)
{
	m_freeIndies.push_back(offset);
}

D3D12_CPU_DESCRIPTOR_HANDLE VT_D3D12::D3D12ResourceDescriptorHeap::getCPUHandle(
	VT::DescriptorBindingHeapOffsetType offset) const
{
	return D3D12_CPU_DESCRIPTOR_HANDLE{ m_baseCPUHeapHandle.ptr + offset * m_descriptorSize };
}

D3D12_GPU_DESCRIPTOR_HANDLE VT_D3D12::D3D12ResourceDescriptorHeap::getGPUHandle(
	VT::DescriptorBindingHeapOffsetType offset) const
{
	return getDesc().m_isShaderVisible ?
		D3D12_GPU_DESCRIPTOR_HANDLE{ m_baseGPUHeapHandle.ptr + offset * m_descriptorSize } :
		D3D12_GPU_DESCRIPTOR_HANDLE{ 0 };
}
