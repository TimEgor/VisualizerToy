#pragma once

#include "D3D12GraphicsPlugin/D3D12ComPtrDefines.h"
#include "D3D12GraphicsPlugin/Synchronization/D3D12Fence.h"
#include "D3D12GraphicsPlugin/Resource/D3D12Resource.h"

#include <vector>

#include "GraphicResourceCommon/IGraphicResource.h"


namespace VT_D3D12
{
	class D3D12UploadingContext final
	{
		struct UploadingResource final
		{
			D3D12ResourceBase m_stagingResource;
			D3D12ResourceBase m_dstResource;
		};

		using UploadingResouceCollection = std::vector<UploadingResource>;

	private:
		UploadingResouceCollection m_uploadingResources;

		D3D12CommandQueueComPtr m_uploadingCommandQueue = nullptr;
		D3D12GraphicsCommandListComPtr m_commandList = nullptr;
		D3D12CommandAllocatorComPtr m_allocator = nullptr;

		D3D12Fence m_syncFence;

		VT::FenceValueType m_lastSubmittedFenceValue = VT::InitialFenceValue;

	public:
		D3D12UploadingContext(D3D12CommandQueueComPtr d3d12UploadingCommandQueue, D3D12GraphicsCommandListComPtr d3d12CommandList,
			D3D12CommandAllocatorComPtr d3d12CommandAllocator, D3D12Fence syncFence);
		~D3D12UploadingContext() { wait(); }

		void reset();
		void submit();
		void wait();

		void uploadResource(D3D12ResourceBase* scrResource, D3D12ResourceBase* dstResource, const D3D12_SUBRESOURCE_DATA& data);
	};
}
