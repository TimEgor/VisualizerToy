#pragma once

#include "GraphicDevice/IGraphicDevice.h"

#include "ManagedObjectStorage.h"
#include "ManagedStorageInfos.h"

namespace VT::ManagedGraphicDevice
{
	class ManagedGraphicDevice : public IGraphicDevice
	{
	public:
		using BufferStorage = ManagedObjectStorageBase<ManagedGPUBufferStorageInfoBase>;

		using Texture2DStorage = ManagedObjectStorageBase<ManagedTexture2DStorageInfoBase>;

		using VertexShaderStorage = ManagedObjectStorageBase<ManagedVertexShaderStorageInfoBase>;
		using PixelShaderStorage = ManagedObjectStorageBase<ManagedPixelShaderStorageInfoBase>;

		using PipelineStateStorage = ManagedObjectStorageBase<ManagedPipelineStateStorageInfoBase>;
		using PipelineBindingLayoutStorage = ManagedObjectStorageBase<ManagedPipelineBindingLayoutStorageInfoBase>;

		using CommandListStorage = ManagedObjectStorageBase<ManagedCommandListStorageInfoBase>;

		using FenceStorage = ManagedObjectStorageBase<ManagedFenceStorageInfoBase>;

		using GraphicResourceDescriptorStorage = ManagedObjectStorageBase<ManagedGraphicResourceDescriptorStorageInfoBase>;

	private:
		BufferStorage* m_bufferStorage = nullptr;

		Texture2DStorage* m_texture2DStorage = nullptr;

		VertexShaderStorage* m_vertexShaderStorage = nullptr;
		PixelShaderStorage* m_pixelShaderStorage = nullptr;

		PipelineStateStorage* m_pipelineStateStorage = nullptr;
		PipelineBindingLayoutStorage* m_pipelineBindingLayoutStorage = nullptr;

		CommandListStorage* m_commandListStorage = nullptr;

		FenceStorage* m_fenceStorage = nullptr;

		GraphicResourceDescriptorStorage* m_descriptorStorage = nullptr;

	protected:
		virtual bool initDevice(bool isSwapChainEnabled) = 0;
		virtual void releaseDevice() = 0;

		//
		virtual bool createShaderResourceDescriptor(ManagedGraphicResourceDescriptorBase* descriptor, IGraphicResource* resource) = 0;
		virtual void destroyShaderResourceDescriptor(ManagedGraphicResourceDescriptorBase* descriptor) = 0;

		//Buffer
		virtual bool createBuffer(ManagedGPUBufferBase* buffer, const GPUBufferDesc& desc,
			GraphicStateValueType initialState, const InitialGPUBufferData* initialData) = 0;
		virtual void destroyBuffer(ManagedGPUBufferBase* buffer) = 0;

		virtual bool createBufferResourceDescriptor(ManagedGraphicResourceDescriptorBase* descriptor, IGPUBuffer* buffer) = 0;
		virtual void destroyBufferResourceDescriptor(ManagedGraphicResourceDescriptorBase* descriptor) = 0;

		//Textures
		virtual bool createTexture2D(ManagedTexture2DBase* texture, const Texture2DDesc& desc, GraphicStateValueType initialState) = 0;
		virtual void destroyTexture2D(ManagedTexture2DBase* texture) = 0;

		virtual bool createRenderTargetDescriptor(ManagedGraphicResourceDescriptorBase* descriptor, ITexture* texture) = 0;
		virtual void destroyRenderTargetDescriptor(ManagedGraphicResourceDescriptorBase* descriptor) = 0;

		virtual bool createDepthStencilDescriptor(ManagedGraphicResourceDescriptorBase* descriptor, ITexture* texture) = 0;
		virtual void destroyDepthStencilDescriptor(ManagedGraphicResourceDescriptorBase* descriptor) = 0;

		//Shaders
		virtual bool createVertexShader(ManagedVertexShaderBase* shader, const void* code, size_t codeSize) = 0;
		virtual void destroyVertexShader(ManagedVertexShaderBase* shader) = 0;

		virtual bool createPixelShader(ManagedPixelShaderBase* shader, const void* code, size_t codeSize) = 0;
		virtual void destroyPixelShader(ManagedPixelShaderBase* shader) = 0;

		//Pipeline
		virtual bool createPipelineState(ManagedPipelineStateBase* state,
			const PipelineStateInfo& info, const IPipelineBindingLayout* bindingLayout, const InputLayoutDesc* inputLayoutDesc) = 0;
		virtual void destroyPipelineState(ManagedPipelineStateBase* state) = 0;

		virtual bool createPipelineBindingLayout(ManagedPipelineBindingLayoutBase* layout,
			const PipelineBindingLayoutDesc& desc) = 0;
		virtual void destroyPipelineBindingLayout(ManagedPipelineBindingLayoutBase* layout) = 0;

		//Commands
		virtual bool createCommandList(ManagedCommandListBase* commandList) = 0;
		virtual void destroyCommandList(ManagedCommandListBase* commandList) = 0;

		//Sync
		virtual bool createFence(ManagedFenceBase* fence) = 0;
		virtual void destroyFence(ManagedFenceBase* fence) = 0;

		virtual BufferStorage* createBufferStorage() const = 0;

		virtual Texture2DStorage* createTexture2DStorage() const = 0;

		virtual VertexShaderStorage* createVertexShaderStorage() const = 0;
		virtual PixelShaderStorage* createPixelShaderStorage() const = 0;

		virtual PipelineStateStorage* createPipelineStateStorage() const = 0;
		virtual PipelineBindingLayoutStorage* createPipelineBindingLayoutStorage() const = 0;

		virtual CommandListStorage* createCommandListStorage() const = 0;

		virtual FenceStorage* createFenceStorage() const = 0;

		virtual GraphicResourceDescriptorStorage* createGraphicResourceDescriptorStorage() const = 0;

	public:
		ManagedGraphicDevice() = default;

		virtual bool init(bool isSwapChainEnabled) override;
		virtual void release() override;

		virtual IGraphicResourceDescriptor* createShaderResourceDescriptor(IGraphicResource* resource) override;
		virtual void destroyShaderResourceDescriptor(IGraphicResourceDescriptor* descriptor) override;

		//Buffer
		virtual IGPUBuffer* createBuffer(const GPUBufferDesc& desc,
			GraphicStateValueType initialState, const InitialGPUBufferData* initialData) override;
		virtual void destroyBuffer(IGPUBuffer* buffer) override;

		virtual IGraphicResourceDescriptor* createBufferResourceDescriptor(IGPUBuffer* buffer) override;
		virtual void destroyBufferResourceDescriptor(IGraphicResourceDescriptor* descriptor) override;

		//Textures
		virtual ITexture2D* createTexture2D(const Texture2DDesc& desc, GraphicStateValueType initialState) override;
		virtual void destroyTexture2D(ITexture2D* texture) override;

		virtual IGraphicResourceDescriptor* createRenderTargetDescriptor(ITexture* texture) override;
		virtual void destroyRenderTargetDescriptor(IGraphicResourceDescriptor* descriptor) override;

		virtual IGraphicResourceDescriptor* createDepthStencilDescriptor(ITexture* texture) override;
		virtual void destroyDepthStencilDescriptor(IGraphicResourceDescriptor* descriptor) override;

		//Shaders
		virtual IVertexShader* createVertexShader(const void* code, size_t codeSize) override;
		virtual void destroyVertexShader(IVertexShader* shader) override;

		virtual IPixelShader* createPixelShader(const void* code, size_t codeSize) override;
		virtual void destroyPixelShader(IPixelShader* shader) override;

		//Pipeline
		virtual IPipelineState* createPipelineState(const PipelineStateInfo& info,
			const IPipelineBindingLayout* bindingLayout, const InputLayoutDesc* inputLayoutDesc) override;
		virtual void destroyPipelineState(IPipelineState* state) override;

		virtual IPipelineBindingLayout* createPipelineBindingLayout(const PipelineBindingLayoutDesc& desc) override;
		virtual void destroyPipelineBindingLayout(IPipelineBindingLayout* layout) override;

		//Command
		virtual ICommandList* createCommandList() override;
		virtual void destroyCommandList(ICommandList* pool) override;

		//Sync
		virtual IFence* createFence() override;
		virtual void destroyFence(IFence* fence) override;
	};
}