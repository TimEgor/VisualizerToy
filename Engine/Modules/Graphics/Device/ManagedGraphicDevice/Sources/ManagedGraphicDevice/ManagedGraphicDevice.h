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

		using Texture2DStorage = ManagedObjectStorageBase<ManagedTexture2DStorageInfoBase>;
		using Texture2DViewStorage = ManagedObjectStorageBase<ManagedTexture2DViewStorageInfoBase>;

		using VertexShaderStorage = ManagedObjectStorageBase<ManagedVertexShaderStorageInfoBase>;
		using PixelShaderStorage = ManagedObjectStorageBase<ManagedPixelShaderStorageInfoBase>;

		using PipelineStateStorage = ManagedObjectStorageBase<ManagedPipelineStateStorageInfoBase>;
		using RenderPassStorage = ManagedObjectStorageBase<ManagedRenderPassStorageInfoBase>;

		using CommandPoolStorage = ManagedObjectStorageBase<ManagedCommandPoolStorageInfoBase>;

	private:
		SwapChainStorage* m_swapChainStorage = nullptr;

		Texture2DStorage* m_texture2DStorage = nullptr;
		Texture2DViewStorage* m_texture2DViewStorage = nullptr;

		VertexShaderStorage* m_vertexShaderStorage = nullptr;
		PixelShaderStorage* m_pixelShaderStorage = nullptr;

		PipelineStateStorage* m_pipelineStateStorage = nullptr;
		RenderPassStorage* m_renderPassStorage = nullptr;

		CommandPoolStorage* m_commandPoolStorage = nullptr;

	protected:
		virtual bool initDevice(bool isSwapChainEnabled) = 0;
		virtual void releaseDevice() = 0;

		virtual bool createSwapChain(ManagedSwapChainBase* swapChain, const SwapChainDesc& desc, const IWindow* window) = 0;
		virtual void destroySwapChain(ManagedSwapChainBase* swapChain) = 0;

		virtual void destroyTexture2D(ManagedTexture2DBase* texture) = 0;
		virtual bool createTexture2DView(ManagedTexture2DViewBase* view, ITexture2D* texture, const TextureViewDesc& desc) = 0;
		virtual void destroyTexture2DView(ManagedTexture2DViewBase* view) = 0;

		virtual bool createVertexShader(ManagedVertexShaderBase* shader, const void* code, size_t codeSize) = 0;
		virtual void destroyVertexShader(ManagedVertexShaderBase* shader) = 0;

		virtual bool createPixelShader(ManagedPixelShaderBase* shader, const void* code, size_t codeSize) = 0;
		virtual void destroyPixelShader(ManagedPixelShaderBase* shader) = 0;

		virtual bool createPipelineState(ManagedPipelineStateBase* state, const PipelineStateInfo& info, const IRenderPass* renderPass) = 0;
		virtual void destroyPipelineState(ManagedPipelineStateBase* state) = 0;

		virtual bool createRenderPass(ManagedRenderPassBase* pass, const RenderPassInfo& info) = 0;
		virtual void destroyRenderPass(ManagedRenderPassBase* pass) = 0;

		virtual bool createCommandPool(ManagedCommandPoolBase* commandPool) = 0;
		virtual void destroyCommandPool(ManagedCommandPoolBase* commandPool) = 0;


		virtual SwapChainStorage* createSwapChainStorage() const = 0;

		virtual Texture2DStorage* createTexture2DStorage() const = 0;
		virtual Texture2DViewStorage* createTexture2DViewStorage() const = 0;

		virtual VertexShaderStorage* createVertexShaderStorage() const = 0;
		virtual PixelShaderStorage* createPixelShaderStorage() const = 0;

		virtual PipelineStateStorage* createPipelineStateStorage() const = 0;
		virtual RenderPassStorage* createRenderPassStorage() const = 0;

		virtual CommandPoolStorage* createCommandPoolStorage() const = 0;

	public:
		ManagedGraphicDevice() = default;

		virtual bool init(bool isSwapChainEnabled) override;
		virtual void release() override;

		virtual ISwapChain* createSwapChain(const SwapChainDesc& desc, const IWindow* window) override;
		virtual void destroySwapChain(ISwapChain* swapChain) override;

		virtual void destroyTexture2D(ITexture2D* texture) override;

		virtual ITexture2DView* createTexture2DView(ITexture2D* texture, const TextureViewDesc& desc) override;
		virtual void destroyTexture(ITexture2DView* view) override;

		virtual IVertexShader* createVertexShader(const void* code, size_t codeSize) override;
		virtual void destroyVertexShader(IVertexShader* shader) override;

		virtual IPixelShader* createPixelShader(const void* code, size_t codeSize) override;
		virtual void destroyPixelShader(IPixelShader* shader) override;

		virtual IPipelineState* createPipelineState(const PipelineStateInfo& info,
			const IRenderPass* renderPasss) override;
		virtual void destroyPipelineState(IPipelineState* state) override;

		virtual IRenderPass* createRenderPass(const RenderPassInfo& info) override;
		virtual void destroyRenderPass(IRenderPass* pass) override;

		virtual ICommandPool* createCommandPool() override;
		virtual void destroyCommandPool(ICommandPool* commandPool) override;
	};
}