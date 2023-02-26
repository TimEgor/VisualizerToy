#pragma once

#include "D3D12GraphicsPlugin/D3D12ComPtrDefines.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedPipelineObjects.h"

namespace VT_D3D12
{
	class D3D12PipelineState
	{
	protected:
		D3D12PipelineStateComPtr m_d3d12PipelineState;

		const VT::PipelineStateHash m_hash = 0;
		const VT::PipelineBindingLayoutHash m_bindingLayoutHash = 0;

	public:
		D3D12PipelineState(D3D12PipelineStateComPtr d3d12PipelineState, VT::PipelineStateHash hash,
			VT::PipelineBindingLayoutHash bindingLayoutHash)
			: m_d3d12PipelineState(d3d12PipelineState), m_hash(hash),
			m_bindingLayoutHash(bindingLayoutHash) {}
		virtual ~D3D12PipelineState() = default;

		D3D12PipelineStateComPtr getD3D12Pipeline() const { return m_d3d12PipelineState; }
	};

	class D3D12GraphicPipelineState final : public VT::ManagedGraphicDevice::ManagedGraphicPipelineStateBase, public D3D12PipelineState
	{
	public:
		D3D12GraphicPipelineState(D3D12PipelineStateComPtr d3d12PipelineState, VT::PipelineStateHash hash,
			VT::PipelineBindingLayoutHash bindingLayoutHash)
			: D3D12PipelineState(d3d12PipelineState, hash, bindingLayoutHash) {}

		virtual VT::PipelineStateHash getHash() const override { return m_hash; }
		virtual VT::PipelineBindingLayoutHash getBindingLayoutHash() const override { return m_bindingLayoutHash; }

		virtual void* getNativeHandle() const override { return getD3D12Pipeline().Get(); }
	};

	class D3D12ComputePipelineState final : public VT::ManagedGraphicDevice::ManagedComputePipelineStateBase, public D3D12PipelineState
	{
	public:
		D3D12ComputePipelineState(D3D12PipelineStateComPtr d3d12PipelineState, VT::PipelineStateHash hash,
			VT::PipelineBindingLayoutHash bindingLayoutHash)
			: D3D12PipelineState(d3d12PipelineState, hash, bindingLayoutHash) {}

		virtual VT::PipelineStateHash getHash() const override { return m_hash; }
		virtual VT::PipelineBindingLayoutHash getBindingLayoutHash() const override { return m_bindingLayoutHash; }

		virtual void* getNativeHandle() const override { return getD3D12Pipeline().Get(); }
	};
}