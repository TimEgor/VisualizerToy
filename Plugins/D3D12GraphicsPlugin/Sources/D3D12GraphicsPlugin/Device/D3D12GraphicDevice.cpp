#include "D3D12GraphicDevice.h"

#include "Core/UtilitiesMacros.h"
#include "Core/String/Format.h"

#include "WindowSystem/IWindow.h"

#include "D3D12GraphicsPlugin/SwapChain/D3D12SwapChain.h"
#include "D3D12GraphicsPlugin/Buffer/D3D12GPUBuffer.h"
#include "D3D12GraphicsPlugin/Commands/D3D12GraphicsCommandList.h"
#include "D3D12GraphicsPlugin/Resource/D3D12ResourceDescriptorHeap.h"
//#include "VulkanGraphicsPlugin/Shaders/VulkanShaders.h"
//#include "VulkanGraphicsPlugin/Pipeline/VulkanPipelineState.h"
//#include "VulkanGraphicsPlugin/Pipeline/VulkanPipelineBindingLayout.h"
#include "D3D12GraphicsPlugin/Synchronization/D3D12Fence.h"
//
#include "D3D12GraphicsPlugin/Utilities/FormatConverter.h"
#include "D3D12GraphicsPlugin/Utilities/DescriptorTypeConverter.h"
//#include "VulkanGraphicsPlugin/Utilities/BufferUsageConverter.h"
//#include "VulkanGraphicsPlugin/Utilities/InputLayoutConverter.h"

#include "InputLayout/InputLayout.h"

bool VT_D3D12::D3D12GraphicDevice::initD3D12Device(bool isSwapChainEnabled)
{
	VT_CHECK_RETURN_FALSE(SUCCEEDED(CreateDXGIFactory2(0, VT_IID_COM(m_dxgiFactory), VT_PPV_COM(m_dxgiFactory))));

#ifdef _DEBUG
	{
		ComPtr<ID3D12Debug> d3d12Debug;
		VT_CHECK_RETURN_FALSE(SUCCEEDED(D3D12GetDebugInterface(VT_IID_COM(d3d12Debug), VT_PPV_COM(d3d12Debug))));
		d3d12Debug->EnableDebugLayer();

		ComPtr<ID3D12Debug1> d3d12Debug1;
		VT_CHECK_RETURN_FALSE(SUCCEEDED(d3d12Debug->QueryInterface(IID_PPV_ARGS(&d3d12Debug1))));
		d3d12Debug1->SetEnableGPUBasedValidation(true);
	}
#endif // DEBUG

	VT_CHECK_RETURN_FALSE(chooseD3D12PhysDevice());

	VT_CHECK_RETURN_FALSE(SUCCEEDED(D3D12CreateDevice(m_dxgiAdapter.Get(), m_featureLevel,
		VT_IID_COM(m_d3d12Device), VT_PPV_COM(m_d3d12Device))));

	return true;
}

VT_D3D12::D3D12CommandQueueComPtr VT_D3D12::D3D12GraphicDevice::createCommandQueue(D3D12_COMMAND_LIST_TYPE commandType)
{
	D3D12_COMMAND_QUEUE_DESC d3d12CommandQueueDesc{};
	d3d12CommandQueueDesc.Type = commandType;

	ID3D12CommandQueue* d3d12CommandQueue;
	m_d3d12Device->CreateCommandQueue(&d3d12CommandQueueDesc, VT_IID(d3d12CommandQueue), VT_PPV(d3d12CommandQueue));

	return d3d12CommandQueue;
}

VT_D3D12::D3D12ResourceDescriptorHeap* VT_D3D12::D3D12GraphicDevice::createResourceDescriptorHeapInternal(
	const VT::GraphicResourceDescriptorHeapDesc& desc)
{
	D3D12_DESCRIPTOR_HEAP_DESC d3d12HeapDesc{};
	d3d12HeapDesc.Type = convertDescriptorTypeVTtoD3D12(desc.m_descriptorType);
	d3d12HeapDesc.NumDescriptors = desc.m_elementsCount;
	d3d12HeapDesc.Flags = desc.m_isShaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	ID3D12DescriptorHeap* d3d12Heap = nullptr;
	HRESULT creationResult = m_d3d12Device->CreateDescriptorHeap(&d3d12HeapDesc, VT_IID(d3d12Heap), VT_PPV(d3d12Heap));
	if (FAILED(creationResult))
	{
		return nullptr;
	}

	const uint32_t descriptorSize = m_d3d12Device->GetDescriptorHandleIncrementSize(d3d12HeapDesc.Type);

	return new D3D12ResourceDescriptorHeap(desc, d3d12Heap, descriptorSize);
}

bool VT_D3D12::D3D12GraphicDevice::chooseD3D12PhysDevice()
{
	assert(m_dxgiFactory);

	IDXGIAdapter1* iterAdapter = nullptr;
	for (uint32_t i = 0; m_dxgiFactory->EnumAdapters1(i, &iterAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		assert(iterAdapter);

		DXGI_ADAPTER_DESC1 adapterDesc{};
		iterAdapter->GetDesc1(&adapterDesc);

		if ((adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 &&
			SUCCEEDED(D3D12CreateDevice(iterAdapter, m_featureLevel, VT_IID(ID3D12Device), nullptr)))
		{
			m_dxgiAdapter = iterAdapter;
			break;
		}
	}

	return m_dxgiAdapter;
}

bool VT_D3D12::D3D12GraphicDevice::initDevice(bool isSwapChainEnabled)
{
	VT_CHECK_RETURN_FALSE(initD3D12Device(isSwapChainEnabled));

	//

	//DxcCreateInstance(CLSID_DxcCompiler, IID_COM(m_dxcShaderCompiler), PPV_COM(m_dxcShaderCompiler));
	//DxcCreateInstance(CLSID_DxcUtils, IID_COM(m_dxcUtils), PPV_COM(m_dxcUtils));

	//
	m_commandQueue = createCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT); // TODO: move to separated abstract class (D3D12CommandQueue : ICommandQueue)
	VT_CHECK_RETURN_FALSE(m_commandQueue);

	//
	VT::GraphicResourceDescriptorHeapDesc descriptorHeapDesc{};

	descriptorHeapDesc.m_elementsCount = 128;
	descriptorHeapDesc.m_descriptorType = VT::GraphicResourceDescriptorType::RTV;
	descriptorHeapDesc.m_isShaderVisible = false;

	m_rtvDescriptorHeap = createResourceDescriptorHeapInternal(descriptorHeapDesc);
	VT_CHECK_RETURN_FALSE(m_rtvDescriptorHeap);

	descriptorHeapDesc.m_elementsCount = 4096;
	descriptorHeapDesc.m_descriptorType = VT::GraphicResourceDescriptorType::SRV;
	descriptorHeapDesc.m_isShaderVisible = true;

	m_srvDescriptorHeap = createResourceDescriptorHeapInternal(descriptorHeapDesc);
	VT_CHECK_RETURN_FALSE(m_srvDescriptorHeap);

	//
	m_globalDeviceFence = reinterpret_cast<D3D12Fence*>(new uint8_t[sizeof(D3D12Fence)]);
	createFence(m_globalDeviceFence);
	VT_CHECK_RETURN_FALSE(m_globalDeviceFence);

	return true;
}

void VT_D3D12::D3D12GraphicDevice::releaseDevice()
{
	if (m_rtvDescriptorHeap)
	{
		destroyGraphicResourceDescriptionHeap(m_rtvDescriptorHeap);
	}

	if (m_srvDescriptorHeap)
	{
		destroyGraphicResourceDescriptionHeap(m_srvDescriptorHeap);
	}
}

void VT_D3D12::D3D12GraphicDevice::update()
{
}

void VT_D3D12::D3D12GraphicDevice::waitIdle()
{
	VT::FenceValueType fenceVal = m_globalDeviceFence->getValue();
	m_commandQueue->Signal(m_globalDeviceFence->getD3D12Fence().Get(), fenceVal);
	m_globalDeviceFence->incrementValue();

	m_globalDeviceFence->wait(fenceVal);
}

VT::ISwapChain* VT_D3D12::D3D12GraphicDevice::createSwapChain(const VT::SwapChainDesc& desc, const VT::IWindow* window)
{
	const VT::WindowSize& windowSize = window->getClientAreaSize();

	HWND windowHandle = reinterpret_cast<HWND>(window->getNativeHandle());

	DXGI_SWAP_CHAIN_DESC1 d3d12SwapChainDesc{};
	d3d12SwapChainDesc.BufferCount = desc.m_imageCount;
	d3d12SwapChainDesc.Format = convertFormatVTtoD3D12(desc.m_format);
	d3d12SwapChainDesc.Width = windowSize.m_width;
	d3d12SwapChainDesc.Height = windowSize.m_height;
	d3d12SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	d3d12SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	d3d12SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	d3d12SwapChainDesc.SampleDesc.Count = 1;
	d3d12SwapChainDesc.SampleDesc.Quality = 0;

	IDXGISwapChain1* d3d12SwapChain = nullptr;

	HRESULT creatingResult = m_dxgiFactory->CreateSwapChainForHwnd(m_commandQueue.Get(), windowHandle, &d3d12SwapChainDesc, nullptr, nullptr, &d3d12SwapChain);
	if (FAILED(creatingResult))
	{
		return false;
	}

	D3D12Texture2D* textures = reinterpret_cast<D3D12Texture2D*>(new uint8_t[sizeof(D3D12Texture2D) * desc.m_imageCount]);
	D3D12ResourceDescriptor* descriptors = reinterpret_cast<D3D12ResourceDescriptor*>(new uint8_t[sizeof(D3D12ResourceDescriptor) * desc.m_imageCount]);

	VT::Texture2DDesc textureDesc{};
	textureDesc.m_format = desc.m_format;
	textureDesc.m_width = windowSize.m_width;
	textureDesc.m_height = windowSize.m_height;

	for (size_t i = 0; i < desc.m_imageCount; ++i)
	{
		ID3D12Resource* d3d12TextureResource = nullptr;

		d3d12SwapChain->GetBuffer(i, VT_IID(d3d12TextureResource), VT_PPV(d3d12TextureResource));
		D3D12Texture2D* d3d12Texture = new (&textures[i]) D3D12Texture2D(textureDesc, d3d12TextureResource);
		createRenderTargetDescriptor(&descriptors[i], d3d12Texture);
	}

	D3D12SwapChain* swapChain = new D3D12SwapChain(desc, m_d3d12Device, d3d12SwapChain, textures, descriptors, desc.m_imageCount);

	return swapChain;
}

void VT_D3D12::D3D12GraphicDevice::destroySwapChain(VT::ISwapChain* swapChain)
{
	assert(swapChain);

	D3D12Texture2D* textures = reinterpret_cast<D3D12Texture2D*>(swapChain->getTargetTexture(0));
	VT_SAFE_DESTROY_ARRAY(textures);

	D3D12ResourceDescriptor* views = reinterpret_cast<D3D12ResourceDescriptor*>(swapChain->getTargetTextureView(0));
	VT_SAFE_DESTROY_ARRAY(views);

	VT_SAFE_DESTROY(swapChain);
}

bool VT_D3D12::D3D12GraphicDevice::createBuffer(VT::ManagedGraphicDevice::ManagedGPUBufferBase* buffer, const VT::GPUBufferDesc& desc)
{
	return false;
}

void VT_D3D12::D3D12GraphicDevice::destroyBuffer(VT::ManagedGraphicDevice::ManagedGPUBufferBase* buffer)
{
	assert(buffer);
}

void VT_D3D12::D3D12GraphicDevice::destroyTexture2D(VT::ManagedGraphicDevice::ManagedTexture2DBase* texture)
{
	assert(texture);
}

bool VT_D3D12::D3D12GraphicDevice::createRenderTargetDescriptor(
	VT::ManagedGraphicDevice::ManagedGraphicResourceDescriptorBase* descriptor, VT::ITexture* texture)
{
	assert(texture);

	D3D12Texture2D* d3d12Texture = reinterpret_cast<D3D12Texture2D*>(texture);

	VT::DescriptorBindingHeapOffsetType offset = m_rtvDescriptorHeap->allocateDescriptor();
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = m_rtvDescriptorHeap->getCPUHandle(offset);
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = m_rtvDescriptorHeap->getGPUHandle(offset);

	m_d3d12Device->CreateRenderTargetView(d3d12Texture->getD3D12Resource().Get(), nullptr, cpuHandle);

	new (descriptor) D3D12ResourceDescriptor(VT::GraphicResourceDescriptorType::RTV, offset, cpuHandle, gpuHandle);

	return true;
}

void VT_D3D12::D3D12GraphicDevice::destroyRenderTargetDescriptor(
	VT::ManagedGraphicDevice::ManagedGraphicResourceDescriptorBase* descriptor)
{
	m_rtvDescriptorHeap->deallocateDescriptor(descriptor->getBindingHeapOffset());
}

bool VT_D3D12::D3D12GraphicDevice::createVertexShader(VT::ManagedGraphicDevice::ManagedVertexShaderBase* shader, 
	const void* code, size_t codeSize)
{
	return false;
}

void VT_D3D12::D3D12GraphicDevice::destroyVertexShader(VT::ManagedGraphicDevice::ManagedVertexShaderBase* shader)
{
}

bool VT_D3D12::D3D12GraphicDevice::createPixelShader(VT::ManagedGraphicDevice::ManagedPixelShaderBase* shader,
	const void* code, size_t codeSize)
{
	return false;
}

void VT_D3D12::D3D12GraphicDevice::destroyPixelShader(VT::ManagedGraphicDevice::ManagedPixelShaderBase* shader)
{
}

bool VT_D3D12::D3D12GraphicDevice::createPipelineState(VT::ManagedGraphicDevice::ManagedPipelineStateBase* state,
	const VT::PipelineStateInfo& info, const VT::IPipelineBindingLayout* bindingLayout, const VT::InputLayoutDesc* inputLayoutDesc)
{
	assert(bindingLayout);
	return false;
}

void VT_D3D12::D3D12GraphicDevice::destroyPipelineState(VT::ManagedGraphicDevice::ManagedPipelineStateBase* state)
{
	assert(state);
}

bool VT_D3D12::D3D12GraphicDevice::createPipelineBindingLayout(
	VT::ManagedGraphicDevice::ManagedPipelineBindingLayoutBase* layout, const VT::PipelineBindingLayoutDesc& desc)
{
	return false;
}

void VT_D3D12::D3D12GraphicDevice::destroyPipelineBindingLayout(VT::ManagedGraphicDevice::ManagedPipelineBindingLayoutBase* layout)
{
}

bool VT_D3D12::D3D12GraphicDevice::createCommandList(VT::ManagedGraphicDevice::ManagedCommandListBase* commandList)
{
	D3D12_COMMAND_LIST_TYPE type = m_commandQueue->GetDesc().Type;

	ID3D12CommandAllocator* d3d12Allocator = nullptr;
	HRESULT creationResult = m_d3d12Device->CreateCommandAllocator(type, VT_IID(d3d12Allocator), VT_PPV(d3d12Allocator));
	if (FAILED(creationResult))
	{
		return false;
	}

	ID3D12GraphicsCommandList* d3d12List = nullptr;
	creationResult = m_d3d12Device->CreateCommandList(0, type, d3d12Allocator, nullptr, VT_IID(d3d12List), VT_PPV(d3d12List));
	if (FAILED(creationResult))
	{
		d3d12Allocator->Release();
		return false;
	}

	d3d12List->Close();

	new (commandList) D3D12GraphicsCommandList(d3d12List, d3d12Allocator);

	return true;
}

void VT_D3D12::D3D12GraphicDevice::destroyCommandList(VT::ManagedGraphicDevice::ManagedCommandListBase* commandList)
{
	assert(commandList);
}

void VT_D3D12::D3D12GraphicDevice::submitCommandList(VT::ICommandList* list, const VT::CommandListSubmitInfo& info)
{
	D3D12GraphicsCommandList* d3d12CommandList = reinterpret_cast<D3D12GraphicsCommandList*>(list);
	ID3D12CommandList* d3d12CommandListHandle = d3d12CommandList->getD3D12CommandList().Get();

	m_commandQueue->ExecuteCommandLists(1, &d3d12CommandListHandle);

	D3D12Fence* d3d12Fence = reinterpret_cast<D3D12Fence*>(info.m_fence);
	if (d3d12Fence)
	{
		m_commandQueue->Signal(d3d12Fence->getD3D12Fence().Get(), d3d12Fence->getValue());
		d3d12Fence->incrementValue();
	}
}

bool VT_D3D12::D3D12GraphicDevice::createFence(VT::ManagedGraphicDevice::ManagedFenceBase* fence)
{
	assert(fence);

	ID3D12Fence* d3d12Fence = nullptr;
	HRESULT creationResult = m_d3d12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, VT_IID(d3d12Fence), VT_PPV(d3d12Fence));
	if (FAILED(creationResult))
	{
		return false;
	}

	HANDLE winEvent = CreateEvent(nullptr, false, false, nullptr);

	new (fence) D3D12Fence(d3d12Fence, winEvent);

	return true;
}

void VT_D3D12::D3D12GraphicDevice::destroyFence(VT::ManagedGraphicDevice::ManagedFenceBase* fence)
{
	assert(fence);

	D3D12Fence* d3d12Fence = reinterpret_cast<D3D12Fence*>(fence);

	CloseHandle(d3d12Fence->getEventHandle());
}

VT::IGraphicResourceDescriptorHeap* VT_D3D12::D3D12GraphicDevice::createGraphicResourceDescriptionHeap(
	const VT::GraphicResourceDescriptorHeapDesc& desc)
{
	return createGraphicResourceDescriptionHeap(desc);
}

void VT_D3D12::D3D12GraphicDevice::destroyGraphicResourceDescriptionHeap(VT::IGraphicResourceDescriptorHeap* heap)
{
	assert(heap);
	VT_SAFE_DESTROY(heap);
}

VT::ManagedGraphicDevice::ManagedGraphicDevice::BufferStorage* VT_D3D12::D3D12GraphicDevice::createBufferStorage() const
{
	//return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedGPUBufferStorageInfo<VulkanGPUBuffer>>();
	return nullptr;
}

VT::ManagedGraphicDevice::ManagedGraphicDevice::Texture2DStorage* VT_D3D12::D3D12GraphicDevice::createTexture2DStorage() const
{
	return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedTexture2DStorageInfo<D3D12Texture2D>>();
}

VT::ManagedGraphicDevice::ManagedGraphicDevice::VertexShaderStorage* VT_D3D12::D3D12GraphicDevice::createVertexShaderStorage() const
{
	//return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedVertexShaderStorageInfo<VulkanVertexShader>>();
	return nullptr;
}

VT::ManagedGraphicDevice::ManagedGraphicDevice::PixelShaderStorage* VT_D3D12::D3D12GraphicDevice::createPixelShaderStorage() const
{
	//return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedPixelShaderStorageInfo<VulkanPixelShader>>();
	return nullptr;
}

VT::ManagedGraphicDevice::ManagedGraphicDevice::PipelineStateStorage* VT_D3D12::D3D12GraphicDevice::createPipelineStateStorage() const
{
	//return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedPipelineStateStorageInfo<VulkanPipelineState>>();
	return nullptr;
}

VT::ManagedGraphicDevice::ManagedGraphicDevice::PipelineBindingLayoutStorage* VT_D3D12::D3D12GraphicDevice::createPipelineBindingLayoutStorage() const
{
	//return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedPipelineBindingLayoutStorageInfo<VulkanPipelineBindingLayout>>;
	return nullptr;
}

VT::ManagedGraphicDevice::ManagedGraphicDevice::CommandListStorage* VT_D3D12::D3D12GraphicDevice::createCommandListStorage() const
{
	return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedCommandListStorageInfo<D3D12GraphicsCommandList>>();
}

VT::ManagedGraphicDevice::ManagedGraphicDevice::FenceStorage* VT_D3D12::D3D12GraphicDevice::createFenceStorage() const
{
	return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedFenceStorageInfo<D3D12Fence>>();
}

VT::ManagedGraphicDevice::ManagedGraphicDevice::GraphicResourceDescriptorStorage*
	VT_D3D12::D3D12GraphicDevice::createGraphicResourceDescriptorStorage() const
{
	return new VT::ManagedGraphicDevice::ManagedObjectStorage<VT::ManagedGraphicDevice::ManagedGraphicResourceDescriptorStorageInfo<D3D12ResourceDescriptor>>();
}
