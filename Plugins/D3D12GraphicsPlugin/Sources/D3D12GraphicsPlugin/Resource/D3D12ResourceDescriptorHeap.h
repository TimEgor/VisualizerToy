#pragma once


#include "D3D12GraphicsPlugin/D3D12ComPtrDefines.h"
#include "GraphicResourceCommon/GraphicResourceDescriptorHeapBase.h"

#include <vector>

namespace VT_D3D12
{
	class D3D12ResourceDescriptorHeap final : public VT::GraphicResourceDescriptorHeapBase
	{
		using HandleIndexContainer = std::vector<VT::DescriptorBindingHeapOffsetType>;

	private:
		HandleIndexContainer m_freeIndies;

		D3D12DescriptorHeapComPtr m_d3d12Heap = nullptr;

		D3D12_CPU_DESCRIPTOR_HANDLE m_baseCPUHeapHandle = {0};
		D3D12_GPU_DESCRIPTOR_HANDLE m_baseGPUHeapHandle = {0};

		uint32_t m_descriptorSize = 0;

	public:
		D3D12ResourceDescriptorHeap(const VT::GraphicResourceDescriptorHeapDesc& desc,
			D3D12DescriptorHeapComPtr d3d12DescriptorHeap, uint32_t descriptorSize);

		virtual VT::DescriptorBindingHeapOffsetType allocateDescriptor() override;
		virtual void deallocateDescriptor(VT::DescriptorBindingHeapOffsetType offset) override;

		D3D12_CPU_DESCRIPTOR_HANDLE getCPUHandle(VT::DescriptorBindingHeapOffsetType offset) const;
		D3D12_GPU_DESCRIPTOR_HANDLE getGPUHandle(VT::DescriptorBindingHeapOffsetType offset) const;

		D3D12DescriptorHeapComPtr getD3D12DescriptorHeap() const { return m_d3d12Heap; }
	};
}
