#pragma once

#include <cstdint>

#include "ManagedGraphicDevice/ManagedObjects/ManagedDescriptorObject.h"

namespace VT_D3D12
{
	constexpr VT::DescriptorBindingHeapOffsetType INVALID_D3D12_DESCRIPTOR_HANDLE_INDEX = -1;

	class D3D12ResourceDescriptor : public VT::ManagedGraphicDevice::ManagedGraphicResourceDescriptorBase
	{
	private:
		//ID3D12Resource* m_d3d12Resource = nullptr;
		VT::DescriptorBindingHeapOffsetType m_handleIndex = INVALID_D3D12_DESCRIPTOR_HANDLE_INDEX;
		D3D12_CPU_DESCRIPTOR_HANDLE m_cpuHandle = { 0 };
		D3D12_GPU_DESCRIPTOR_HANDLE m_gpuHandle = { 0 };

	public:
		D3D12ResourceDescriptor(VT::GraphicResourceDescriptorType type, VT::DescriptorBindingHeapOffsetType handleIndex,
			D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle)
			: ManagedGraphicResourceDescriptorBase(type), m_handleIndex(handleIndex),
			m_cpuHandle(cpuHandle), m_gpuHandle(gpuHandle) {}

		D3D12_CPU_DESCRIPTOR_HANDLE getCPUHandle() const { return m_cpuHandle; }
		D3D12_GPU_DESCRIPTOR_HANDLE getGPUHandle() const { return m_gpuHandle; }

		virtual VT::DescriptorBindingHeapOffsetType getBindingHeapOffset() const override { return m_handleIndex; }
		virtual bool isValid() const override { return m_handleIndex != INVALID_D3D12_DESCRIPTOR_HANDLE_INDEX; }
	};
}
