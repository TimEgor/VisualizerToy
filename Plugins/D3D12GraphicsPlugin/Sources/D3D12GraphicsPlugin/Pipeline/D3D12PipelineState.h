#pragma once

#include "D3D12GraphicsPlugin/D3D12ComPtrDefines.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedPipelineObjects.h"

namespace VT_D3D12
{
	class D3D12PipelineState final : public VT::ManagedGraphicDevice::ManagedPipelineStateBase
	{
	private:
		D3D12PipelineStateComPtr m_d3d12PipelineState = 0;

		const VT::PipelineStateHash m_hash = 0;
		const VT::PipelineBindingLayoutHash m_bindingLayoutHash = 0;

	public:
		D3D12PipelineState(D3D12PipelineStateComPtr d3d12PipelineState, VT::PipelineStateHash hash,
			VT::PipelineBindingLayoutHash bindingLayoutHash)
			: m_d3d12PipelineState(d3d12PipelineState), m_hash(hash),
			m_bindingLayoutHash(bindingLayoutHash) {}

		D3D12PipelineStateComPtr getD3D12Pipeline() const { return m_d3d12PipelineState; }

		virtual VT::PipelineStateHash getHash() const override { return m_hash; }
		virtual VT::PipelineBindingLayoutHash getBindingLayoutHash() const override { return m_bindingLayoutHash; }
	};
}