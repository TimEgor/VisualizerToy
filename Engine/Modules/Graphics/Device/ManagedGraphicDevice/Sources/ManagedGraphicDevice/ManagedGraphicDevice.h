#pragma once

#include "GraphicDevice/IGraphicDevice.h"

#include "ManagedObjectStorage.h"
#include "ManagedStorageInfos.h"

namespace VT::ManagedGraphicDevice
{
	class ManagedGraphicDevice : public IGraphicDevice
	{
	public:
		using SwapChainStorage = ManagedObjectStorageBase<ManagedSwapChainStorageInfoBase>;

		using BufferStorage = ManagedObjectStorageBase<ManagedGPUBufferStorageInfoBase>;

		using Texture2DStorage = ManagedObjectStorageBase<ManagedTexture2DStorageInfoBase>;
		using Texture2DViewStorage = ManagedObjectStorageBase<ManagedTexture2DViewStorageInfoBase>;

		using VertexShaderStorage = ManagedObjectStorageBase<ManagedVertexShaderStorageInfoBase>;
		using PixelShaderStorage = ManagedObjectStorageBase<ManagedPixelShaderStorageInfoBase>;

		using PipelineStateStorage = ManagedObjectStorageBase<ManagedPipelineStateStorageInfoBase>;

		using CommandPoolStorage = ManagedObjectStorageBase<ManagedCommandPoolStorageInfoBase>;
		using CommandListStorage = ManagedObjectStorageBase<ManagedCommandListStorageInfoBase>;

		using FenceStorage = ManagedObjectStorageBase<ManagedFenceStorageInfoBase>;
		using SemaphoreStorage = ManagedObjectStorageBase<ManagedSemaphoreStorageInfoBase>;

	private:
		SwapChainStorage* m_swapChainStorage = nullptr;

		BufferStorage* m_bufferStorage = nullptr;

		Texture2DStorage* m_texture2DStorage = nullptr;
		Texture2DViewStorage* m_texture2DViewStorage = nullptr;

		VertexShaderStorage* m_vertexShaderStorage = nullptr;
		PixelShaderStorage* m_pixelShaderStorage = nullptr;

		PipelineStateStorage* m_pipelineStateStorage = nullptr;

		CommandPoolStorage* m_commandPoolStorage = nullptr;
		CommandListStorage* m_commandListStorage = nullptr;

		FenceStorage* m_fenceStorage = nullptr;
		SemaphoreStorage* m_semaphoreStorage = nullptr;

	protected:
		virtual bool initDevice(bool isSwapChainEnabled) = 0;
		virtual void releaseDevice() = 0;

		//SwapChain
		virtual bool createSwapChain(ManagedSwapChainBase* swapChain, const SwapChainDesc& desc, const IWindow* window) = 0;
		virtual void destroySwapChain(ManagedSwapChainBase* swapChain) = 0;

		//Buffer
		virtual bool createBuffer(ManagedGPUBufferBase* buffer, const GPUBufferDesc& desc) = 0;
		virtual void destroyBuffer(ManagedGPUBufferBase* buffer) = 0;

		//Textures
		virtual void destroyTexture2D(ManagedTexture2DBase* texture) = 0;
		virtual bool createTexture2DView(ManagedTexture2DViewBase* view, ITexture2D* texture, const TextureViewDesc& desc) = 0;
		virtual void destroyTexture2DView(ManagedTexture2DViewBase* view) = 0;

		//Shaders
		virtual bool createVertexShader(ManagedVertexShaderBase* shader, const void* code, size_t codeSize) = 0;
		virtual void destroyVertexShader(ManagedVertexShaderBase* shader) = 0;

		virtual bool createPixelShader(ManagedPixelShaderBase* shader, const void* code, size_t codeSize) = 0;
		virtual void destroyPixelShader(ManagedPixelShaderBase* shader) = 0;

		//Pipeline
		virtual bool createPipelineState(ManagedPipelineStateBase* state,
			const PipelineStateInfo& info, const InputLayoutDesc* inputLayoutDesc) = 0;
		virtual void destroyPipelineState(ManagedPipelineStateBase* state) = 0;

		//Commands
		virtual bool createCommandPool(ManagedCommandPoolBase* commandPool) = 0;
		virtual void destroyCommandPool(ManagedCommandPoolBase* commandPool) = 0;

		virtual bool allocateCommandList(ManagedCommandListBase* commandList, ICommandPool* pool) = 0;

		//Sync
		virtual bool createFence(ManagedFenceBase* fence, bool signaled) = 0;
		virtual void destroyFence(ManagedFenceBase* fence) = 0;

		virtual bool createSemaphore(ManagedSemaphoreBase* semaphore) = 0;
		virtual void destroySemaphore(ManagedSemaphoreBase* semaphore) = 0;


		virtual SwapChainStorage* createSwapChainStorage() const = 0;

		virtual BufferStorage* createBufferStorage() const = 0;

		virtual Texture2DStorage* createTexture2DStorage() const = 0;
		virtual Texture2DViewStorage* createTexture2DViewStorage() const = 0;

		virtual VertexShaderStorage* createVertexShaderStorage() const = 0;
		virtual PixelShaderStorage* createPixelShaderStorage() const = 0;

		virtual PipelineStateStorage* createPipelineStateStorage() const = 0;

		virtual CommandPoolStorage* createCommandPoolStorage() const = 0;
		virtual CommandListStorage* createCommandListStorage() const = 0;

		virtual FenceStorage* createFenceStorage() const = 0;
		virtual SemaphoreStorage* createSemaphoreStorage() const = 0;

	public:
		ManagedGraphicDevice() = default;

		virtual bool init(bool isSwapChainEnabled) override;
		virtual void release() override;

		//SwapChain
		virtual ISwapChain* createSwapChain(const SwapChainDesc& desc, const IWindow* window) override;
		virtual void destroySwapChain(ISwapChain* swapChain) override;

		//Buffer
		virtual IGPUBuffer* createBuffer(const GPUBufferDesc& desc) override;
		virtual void destroyBuffer(IGPUBuffer* buffer) override;

		//Textures
		virtual void destroyTexture2D(ITexture2D* texture) override;

		virtual ITexture2DView* createTexture2DView(ITexture2D* texture, const TextureViewDesc& desc) override;
		virtual void destroyTexture2DView(ITexture2DView* view) override;

		//Shaders
		virtual IVertexShader* createVertexShader(const void* code, size_t codeSize) override;
		virtual void destroyVertexShader(IVertexShader* shader) override;

		virtual IPixelShader* createPixelShader(const void* code, size_t codeSize) override;
		virtual void destroyPixelShader(IPixelShader* shader) override;

		//Pipeline
		virtual IPipelineState* createPipelineState(const PipelineStateInfo& info, const InputLayoutDesc* inputLayoutDesc) override;
		virtual void destroyPipelineState(IPipelineState* state) override;

		//Command
		virtual ICommandPool* createCommandPool() override;
		virtual void destroyCommandPool(ICommandPool* commandPool) override;

		virtual ICommandList* allocateCommandList(ICommandPool* pool) override;

		//Sync
		virtual IFence* createFence(bool signaled) override;
		virtual void destroyFence(IFence* fence) override;

		virtual ISemaphore* createSemaphore() override;
		virtual void destroySemaphore(ISemaphore* semaphore) override;
	};
}