#pragma once

#include "D3D12Core.h"

namespace VT_D3D12
{
	using D3D12DeviceComPtr = ComPtr<ID3D12Device>;
	using D3D12CommandQueueComPtr = ComPtr<ID3D12CommandQueue>;

	using D3D12ResourceComPtr = ComPtr<ID3D12Resource>;
	using D3D12DescriptorHeapComPtr = ComPtr<ID3D12DescriptorHeap>;

	using D3D12CommandAllocatorComPtr = ComPtr<ID3D12CommandAllocator>;
	using D3D12GraphicsCommandListComPtr = ComPtr<ID3D12GraphicsCommandList>;
	using D3D12CommandQueueComPtr = ComPtr<ID3D12CommandQueue>;

	using D3D12PipelineStateComPtr = ComPtr<ID3D12PipelineState>;
	using D3D12RootSignatureComPtr = ComPtr<ID3D12RootSignature>;

	using D3D12FenceComPtr = ComPtr<ID3D12Fence>;

	using D3D12BlobComPtr = ComPtr<ID3DBlob>;

	using DXGIFactoryComPtr = ComPtr<IDXGIFactory2>;
	using DXGIAdapterComPtr = ComPtr<IDXGIAdapter1>;
	using DXGISwapChainComPtr = ComPtr<IDXGISwapChain>;
}