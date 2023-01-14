#pragma once

#include "D3D12DeviceObject.h"
#include "ManagedGraphicDevice/ManagedGraphicDevice.h"

#define VT_GRAPHIC_DEVICE_D3D12_TYPE

namespace VT_D3D12
{
	class D3D12Fence;
	class D3D12ResourceDescriptorHeap;

	class D3D12GraphicDevice final : public VT::ManagedGraphicDevice::ManagedGraphicDevice
	{
	private:
		DXGIFactoryComPtr m_dxgiFactory = nullptr;
		DXGIAdapterComPtr m_dxgiAdapter = nullptr;

		D3D12DeviceComPtr m_d3d12Device = nullptr;

		D3D12CommandQueueComPtr m_commandQueue = nullptr; //TODO: moved from here to separated object

		D3D12ResourceDescriptorHeap* m_rtvDescriptorHeap = nullptr;
		D3D12ResourceDescriptorHeap* m_srvDescriptorHeap = nullptr;

		D3D12Fence* m_globalDeviceFence = nullptr;

		D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_12_1;

		bool chooseD3D12PhysDevice();
		bool initD3D12Device(bool isSwapChainEnabled);

		D3D12CommandQueueComPtr createCommandQueue(D3D12_COMMAND_LIST_TYPE commandType);

		D3D12ResourceDescriptorHeap* createResourceDescriptorHeapInternal(const VT::GraphicResourceDescriptorHeapDesc& desc);

	protected:
		virtual bool initDevice(bool isSwapChainEnabled) override;
		virtual void releaseDevice() override;

		//SwapChain
		virtual VT::ISwapChain* createSwapChain(const VT::SwapChainDesc& desc, const VT::IWindow* window) override;
		virtual void destroySwapChain(VT::ISwapChain* swapChain) override;

		//Buffer
		virtual bool createBuffer(VT::ManagedGraphicDevice::ManagedGPUBufferBase* buffer, const VT::GPUBufferDesc& desc) override;
		virtual void destroyBuffer(VT::ManagedGraphicDevice::ManagedGPUBufferBase* buffer) override;

		//Textures
		virtual void destroyTexture2D(VT::ManagedGraphicDevice::ManagedTexture2DBase* texture) override;

		virtual bool createRenderTargetDescriptor(VT::ManagedGraphicDevice::ManagedGraphicResourceDescriptorBase* descriptor, VT::ITexture* texture) override;
		virtual void destroyRenderTargetDescriptor(VT::ManagedGraphicDevice::ManagedGraphicResourceDescriptorBase* descriptor) override;

		//Shaders
		virtual bool createVertexShader(VT::ManagedGraphicDevice::ManagedVertexShaderBase* shader, const void* code, size_t codeSize) override;
		virtual void destroyVertexShader(VT::ManagedGraphicDevice::ManagedVertexShaderBase* shader) override;

		virtual bool createPixelShader(VT::ManagedGraphicDevice::ManagedPixelShaderBase* shader, const void* code, size_t codeSize) override;
		virtual void destroyPixelShader(VT::ManagedGraphicDevice::ManagedPixelShaderBase* shader) override;

		//Pipeline
		virtual bool createPipelineState(VT::ManagedGraphicDevice::ManagedPipelineStateBase* state,
			const VT::PipelineStateInfo& info, const VT::IPipelineBindingLayout* bindingLayout, const VT::InputLayoutDesc* inputLayoutDesc) override;
		virtual void destroyPipelineState(VT::ManagedGraphicDevice::ManagedPipelineStateBase* state) override;

		virtual bool createPipelineBindingLayout(VT::ManagedGraphicDevice::ManagedPipelineBindingLayoutBase* layout,
		    const VT::PipelineBindingLayoutDesc& desc) override;
		virtual void destroyPipelineBindingLayout(VT::ManagedGraphicDevice::ManagedPipelineBindingLayoutBase* layout) override;

		//Commands
		virtual bool createCommandList(VT::ManagedGraphicDevice::ManagedCommandListBase* commandList) override;
		virtual void destroyCommandList(VT::ManagedGraphicDevice::ManagedCommandListBase* commandList) override;

		virtual void submitCommandList(VT::ICommandList* list, const VT::CommandListSubmitInfo& info) override;

		//Sync
		virtual bool createFence(VT::ManagedGraphicDevice::ManagedFenceBase* fence) override;
		virtual void destroyFence(VT::ManagedGraphicDevice::ManagedFenceBase* fence) override;

		virtual VT::IGraphicResourceDescriptorHeap* createGraphicResourceDescriptionHeap(
			const VT::GraphicResourceDescriptorHeapDesc& desc) override;
		virtual void destroyGraphicResourceDescriptionHeap(VT::IGraphicResourceDescriptorHeap* heap) override;

		virtual BufferStorage* createBufferStorage() const override;

		virtual Texture2DStorage* createTexture2DStorage() const override;

		virtual VertexShaderStorage* createVertexShaderStorage() const override;
		virtual PixelShaderStorage* createPixelShaderStorage() const override;

		virtual PipelineStateStorage* createPipelineStateStorage() const override;
		virtual PipelineBindingLayoutStorage* createPipelineBindingLayoutStorage() const override;

		virtual CommandListStorage* createCommandListStorage() const override;

		virtual FenceStorage* createFenceStorage() const override;

		virtual GraphicResourceDescriptorStorage* createGraphicResourceDescriptorStorage() const override;

	public:
		D3D12GraphicDevice() = default;

		virtual void update() override;

		virtual void waitIdle() override;

		VT_GRAPHIC_DEVICE_TYPE(VT_GRAPHIC_DEVICE_D3D12_TYPE)
	};
}