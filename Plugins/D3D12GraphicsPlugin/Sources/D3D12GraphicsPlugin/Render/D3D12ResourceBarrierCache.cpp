#include "D3D12ResourceBarrierCache.h"

bool VT_D3D12::D3D12ResourceBarrierCache::init(size_t maxCount)
{
	if (maxCount == 0)
	{
		return false;
	}

	m_maxCount = maxCount;
	m_barriers.reserve(maxCount);

	return true;
}

void VT_D3D12::D3D12ResourceBarrierCache::release()
{
	m_maxCount = 0;
	m_barriers = BarrierCollection();
}

void VT_D3D12::D3D12ResourceBarrierCache::addBarrier(D3D12GraphicsCommandListComPtr commandList,
	const D3D12_RESOURCE_TRANSITION_BARRIER& barrier)
{
	D3D12_RESOURCE_BARRIER& newBarrier = m_barriers.emplace_back();
	newBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	newBarrier.Transition = barrier;

	if (m_barriers.size() == m_maxCount)
	{
		flush(commandList);
	}
}

void VT_D3D12::D3D12ResourceBarrierCache::flush(D3D12GraphicsCommandListComPtr commandList)
{
	if (!m_barriers.empty())
	{
		commandList->ResourceBarrier(m_barriers.size(), m_barriers.data());
		m_barriers.clear();
	}
}
