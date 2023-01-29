#pragma once

#include "GraphicCommandResources/ICommandQueue.h"
#include "D3D12GraphicsPlugin/D3D12ComPtrDefines.h"

namespace VT_D3D12
{
	class D3D12CommandQueue final : public VT::ICommandQueue
	{
	private:
		D3D12CommandQueueComPtr m_d3d12Queue = nullptr;

	public:
		D3D12CommandQueue(D3D12CommandQueueComPtr d3d12CommandQueue)
			: m_d3d12Queue(d3d12CommandQueue) {}

		D3D12CommandQueueComPtr getD3D12CommandQueue() const { return m_d3d12Queue; }

		virtual void* getNativeHandle() const override { return getD3D12CommandQueue().Get(); }
	};
}