#pragma once

#include "D3D12GraphicsPlugin/D3D12ComPtrDefines.h"

namespace VT_D3D12
{
	class D3D12ResourceBarrierCache final
	{
		using BarrierCollection = std::vector<D3D12_RESOURCE_BARRIER>;

	private:
		 BarrierCollection m_barriers;
		 size_t m_maxCount = 0;

	public:
		D3D12ResourceBarrierCache() = default;

		bool init(size_t maxCount);
		void release();

		void addBarrier(D3D12GraphicsCommandListComPtr commandList, const D3D12_RESOURCE_TRANSITION_BARRIER& barrier);
		void flush(D3D12GraphicsCommandListComPtr commandList);
	};
}
