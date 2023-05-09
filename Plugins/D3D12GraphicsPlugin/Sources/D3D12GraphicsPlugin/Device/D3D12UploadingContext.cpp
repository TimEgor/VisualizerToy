#include "D3D12UploadingContext.h"

#include "D3D12GraphicsPlugin/Resource/D3D12Resource.h"

#include "Profile/Profile.h"

VT_D3D12::D3D12UploadingContext::D3D12UploadingContext(D3D12CommandQueueComPtr d3d12UploadingCommandQueue,
                                                       D3D12GraphicsCommandListComPtr d3d12CommandList, D3D12CommandAllocatorComPtr d3d12CommandAllocator, D3D12Fence syncFence)
	: m_uploadingCommandQueue(d3d12UploadingCommandQueue),
	m_commandList(d3d12CommandList), m_allocator(d3d12CommandAllocator), m_syncFence(syncFence)
{
	
}

void VT_D3D12::D3D12UploadingContext::reset()
{
	m_uploadingResources.clear();

	m_allocator->Reset();
	m_commandList->Reset(m_allocator.Get(), nullptr);
}

void VT_D3D12::D3D12UploadingContext::submit()
{
	m_commandList->Close();

	if (m_uploadingResources.empty())
	{
		return;
	}

	m_lastSubmittedFenceValue = m_syncFence.getValue();

	ID3D12CommandList* d3d12CommandListHandle = m_commandList.Get();
	m_uploadingCommandQueue->ExecuteCommandLists(1, &d3d12CommandListHandle);
	m_uploadingCommandQueue->Signal(m_syncFence.getD3D12Fence().Get(), m_syncFence.getValue());
	m_syncFence.incrementValue();
}

void VT_D3D12::D3D12UploadingContext::wait()
{
	VT_PROFILE_BLOCK_EVENT("Wait GPU Context");
	m_syncFence.wait(m_lastSubmittedFenceValue);
}

void VT_D3D12::D3D12UploadingContext::uploadResource(D3D12ResourceBase* scrResource, D3D12ResourceBase* dstResource,
	const D3D12_SUBRESOURCE_DATA& data)
{
	m_uploadingResources.emplace_back(UploadingResource{ *scrResource, *dstResource });

	ID3D12Resource* d3d12DstResource = dstResource->getD3D12Resource().Get();

	UpdateSubresources(m_commandList.Get(), d3d12DstResource, scrResource->getD3D12Resource().Get(), 0, 0, 1, &data);
}
