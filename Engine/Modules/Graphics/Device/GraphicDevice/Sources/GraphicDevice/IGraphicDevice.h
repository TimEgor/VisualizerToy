#pragma once

#include "Core/TypeHashMacros.h"

namespace VT
{
	class IWindow;
	class ISwapChain;

	class IGraphicResource;

	class IGPUBuffer;

	class ITexture;
	class ITexture2D;

	class IVertexShader;
	class IPixelShader;

	class IPipelineState;
	class IPipelineBindingLayout;

	class ICommandList;

	class IFence;

	class IGraphicResourceDescriptor;
	class IGraphicResourceDescriptorHeap;

	struct SwapChainDesc;

	struct GPUBufferDesc;

	struct TextureViewDesc;

	struct PipelineStateInfo;
	struct PipelineBindingLayoutDesc;
	struct InputLayoutDesc;

	struct GraphicResourceDescriptorHeapDesc;

	struct CommandListSubmitInfo final
	{
		IFence* m_fence = nullptr;
	};

	using GraphicDeviceType = HashTyped::Type;
	using GraphicDeviceTypeHash = uint32_t;

	class IGraphicDevice : public HashTyped
	{
	public:
		IGraphicDevice() = default;
		virtual ~IGraphicDevice() {}

		virtual bool init(bool isSwapChainEnabled) = 0;
		virtual void release() = 0;

		virtual void update() = 0;

		virtual void waitIdle() = 0;

		//SwapChain
		virtual ISwapChain* createSwapChain(const SwapChainDesc& desc, const IWindow* window) = 0;
		virtual void destroySwapChain(ISwapChain* swapChain) = 0;

		//
		virtual IGraphicResourceDescriptor* createShaderResourceDescriptor(IGraphicResource* resource) = 0;
		virtual void destroyShaderResourceDescriptor(IGraphicResourceDescriptor* descriptor) = 0;

		//Buffers
		virtual IGPUBuffer* createBuffer(const GPUBufferDesc& desc) = 0;
		virtual void destroyBuffer(IGPUBuffer* buffer) = 0;

		virtual IGraphicResourceDescriptor* createBufferResourceDescriptor(IGPUBuffer* buffer) = 0;
		virtual void destroyBufferResourceDescriptor(IGraphicResourceDescriptor* descriptor) = 0;

		//Texture
		virtual void destroyTexture2D(ITexture2D* texture) = 0;

		virtual IGraphicResourceDescriptor* createRenderTargetDescriptor(ITexture* texture) = 0;
		virtual void destroyRenderTargetDescriptor(IGraphicResourceDescriptor* descriptor) = 0;

		//Shaders
		virtual IVertexShader* createVertexShader(const void* code, size_t codeSize) = 0;
		virtual void destroyVertexShader(IVertexShader* shader) = 0;

		virtual IPixelShader* createPixelShader(const void* code, size_t codeSize) = 0;
		virtual void destroyPixelShader(IPixelShader* shader) = 0;

		//Pipeline
		virtual IPipelineState* createPipelineState(const PipelineStateInfo& info,
			const IPipelineBindingLayout* bindingLayout, const InputLayoutDesc* inputLayoutDesc) = 0;
		virtual void destroyPipelineState(IPipelineState* state) = 0;

		virtual IPipelineBindingLayout* createPipelineBindingLayout(const PipelineBindingLayoutDesc& desc) = 0;
		virtual void destroyPipelineBindingLayout(IPipelineBindingLayout* layout) = 0;

		//Commands
		virtual ICommandList* createCommandList() = 0;
		virtual void destroyCommandList(ICommandList* pool) = 0;

		virtual void submitCommandList(ICommandList* list, const CommandListSubmitInfo& info) = 0;

		// Synchronization
		virtual IFence* createFence() = 0;
		virtual void destroyFence(IFence* fence) = 0;

		//
		virtual IGraphicResourceDescriptorHeap* getBindlessResourceDescriptionHeap() = 0;
		virtual IGraphicResourceDescriptorHeap* createGraphicResourceDescriptionHeap(const GraphicResourceDescriptorHeapDesc& desc) = 0;
		virtual void destroyGraphicResourceDescriptionHeap(IGraphicResourceDescriptorHeap* heap) = 0;

		//
		virtual void setResourceName(IGraphicResource* resource, const char* name) = 0;

		virtual GraphicDeviceType getType() const = 0;
	};
}

#define VT_GRAPHIC_DEVICE_TYPE(DEVICE_TYPE) VT_HASH_TYPE(#DEVICE_TYPE, VT::GraphicDeviceType, GraphicDevice)