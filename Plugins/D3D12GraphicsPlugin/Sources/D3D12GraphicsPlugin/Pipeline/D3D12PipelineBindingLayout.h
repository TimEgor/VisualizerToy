#pragma once

#include "D3D12GraphicsPlugin/D3D12ComPtrDefines.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedPipelineObjects.h"

namespace VT_D3D12
{
	class D3D12PipelineBindingLayout final : public VT::ManagedGraphicDevice::ManagedPipelineBindingLayoutBase
	{
	private:
		D3D12RootSignatureComPtr m_d3d12RootSignature = nullptr;

		VT::PipelineBindingLayoutHash m_hash = 0;

	public:
		D3D12PipelineBindingLayout(D3D12RootSignatureComPtr d3d12RootSignature, VT::PipelineBindingLayoutHash hash)
			: m_d3d12RootSignature(d3d12RootSignature), m_hash(hash) {}

		D3D12RootSignatureComPtr getD3D12RootSignature() const { return m_d3d12RootSignature; }
		D3D12RootSignatureComPtr& getD3D12RootSignatureRef() { return m_d3d12RootSignature; }

		virtual VT::PipelineBindingLayoutHash getHash() const override { return m_hash; }

		virtual void* getNativeHandle() const override { return getD3D12RootSignature().Get(); }
	};
}