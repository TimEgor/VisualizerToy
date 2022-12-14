#pragma once

#include "Core/TypeHashMacros.h"

namespace VT
{
	class IWindow;
	class ISwapChain;

	class IGPUBuffer;

	class ITexture2D;
	class ITexture2DView;

	class IVertexShader;
	class IPixelShader;

	class IPipelineState;

	class ICommandPool;
	class ICommandList;

	class IFence;
	class ISemaphore;


	struct SwapChainDesc;

	struct GPUBufferDesc;

	struct TextureViewDesc;

	struct PipelineStateInfo;
	struct InputLayoutDesc;
	struct RenderPassInfo;

	struct CommandListSubmitInfo final
	{
		IFence* m_fence = nullptr;
		ISemaphore* m_waitSemaphore = nullptr;
		ISemaphore* m_completeSemaphore = nullptr;
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

		virtual void wait() = 0;

		//SwapChain
		virtual ISwapChain* createSwapChain(const SwapChainDesc& desc, const IWindow* window) = 0;
		virtual void destroySwapChain(ISwapChain* swapChain) = 0;

		//Buffers
		virtual IGPUBuffer* createBuffer(const GPUBufferDesc& desc) = 0;
		virtual void destroyBuffer(IGPUBuffer* buffer) = 0;

		//Texture
		virtual void destroyTexture2D(ITexture2D* texture) = 0;

		virtual ITexture2DView* createTexture2DView(ITexture2D* texture, const TextureViewDesc& desc) = 0;
		virtual void destroyTexture2DView(ITexture2DView* view) = 0;

		//Shaders
		virtual IVertexShader* createVertexShader(const void* code, size_t codeSize) = 0;
		virtual void destroyVertexShader(IVertexShader* shader) = 0;

		virtual IPixelShader* createPixelShader(const void* code, size_t codeSize) = 0;
		virtual void destroyPixelShader(IPixelShader* shader) = 0;

		//Pipeline
		virtual IPipelineState* createPipelineState(const PipelineStateInfo& info, const InputLayoutDesc* inputLayoutDesc) = 0;
		virtual void destroyPipelineState(IPipelineState* state) = 0;


		//Commands
		virtual ICommandPool* createCommandPool() = 0;
		virtual void destroyCommandPool(ICommandPool* commandPool) = 0;

		virtual ICommandList* allocateCommandList(ICommandPool* pool) = 0;

		virtual void submitCommandList(ICommandList* list, const CommandListSubmitInfo& info) = 0;

		// Synchronization
		virtual IFence* createFence(bool signaled) = 0;
		virtual void destroyFence(IFence* fence) = 0;

		virtual void waitFences(uint32_t count, IFence* fences) = 0;
		virtual void resetFences(uint32_t count, IFence* fences) = 0;

		virtual ISemaphore* createSemaphore() = 0;
		virtual void destroySemaphore(ISemaphore* semaphore) = 0;

		virtual GraphicDeviceType getType() const = 0;
	};
}

#define VT_GRAPHIC_DEVICE_TYPE(DEVICE_TYPE) VT_HASH_TYPE(#DEVICE_TYPE, VT::GraphicDeviceType, GraphicDevice)