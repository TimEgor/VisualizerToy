#pragma once

#include "D3D12GraphicsPlugin/D3D12ComPtrDefines.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedSyncObjects.h"

namespace VT_D3D12
{
	class D3D12Fence final : public VT::ManagedGraphicDevice::ManagedFenceBase
	{
	private:
		D3D12FenceComPtr m_d3d12Fence = nullptr;
		HANDLE m_winEvent = nullptr;

		VT::FenceValueType m_value = 1;

	public:
		D3D12Fence(D3D12FenceComPtr d3d12Fence, HANDLE winEvent)
			: m_d3d12Fence(d3d12Fence), m_winEvent(winEvent) {}

		virtual void wait(VT::FenceValueType value) override;
		virtual VT::FenceValueType getValue() const override { return m_value; }

		void incrementValue() { ++m_value; }

		D3D12FenceComPtr getD3D12Fence() const { return m_d3d12Fence; }
		D3D12FenceComPtr& getD3D12FenceRef() { return m_d3d12Fence; }
		HANDLE getEventHandle() const { return m_winEvent; }
	};
}