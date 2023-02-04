#pragma once

#include "GraphicResourceManager/IGraphicResourceManager.h"
#include "ObjectPool/ThreadSafeObjectPool.h"
#include "NamedReferencePool/NamedObjectPoolHandle.h"
#include "InputLayoutCollection.h"
#include "NativeHandleHashPoolCollection.h"
#include "ManagedObjectHandles/ManagedResourceHandles.h"

namespace VT
{
	class IPipelineBindingLayout;

	class GraphicResourceManager : public IGraphicResourceManager
	{
		friend ManagedShaderGraphicResourceViewHandle;

		friend ManagedGPUBufferGraphicResourceHandle;

		friend ManagedTexture2DResourceHandle;

		friend ManagedVertexShaderGraphicObjectHandle;
		friend ManagedPixelShaderGraphicObjectHandle;

		friend ManagedPipelineStateGraphicObjectHandle;
		friend ManagedPipelineBindingLayoutGraphicObjectHandle;

		friend class ManagedInputLayoutHandle;

		using ShaderResourceViewPool = ThreadSafeObjectPool<ManagedShaderGraphicResourceViewHandle, ObjectPoolHandle32>;

		using GPUBufferPool = ThreadSafeObjectPool<ManagedGPUBufferGraphicResourceHandle, ObjectPoolHandle32>;

		using Texture2DPool = ThreadSafeObjectPool<ManagedTexture2DResourceHandle, NamedObjectPoolHandle32>;

		using VertexShaderPool = ThreadSafeObjectPool<ManagedVertexShaderGraphicObjectHandle, NamedObjectPoolHandle32>;
		using PixelShaderPool = ThreadSafeObjectPool<ManagedPixelShaderGraphicObjectHandle, NamedObjectPoolHandle32>;

		using PipelineStateCollection = NativeHandleHashPoolCollection<ManagedPipelineStateGraphicObjectHandle,
			PipelineStateReference, PipelineStateConstReference, PipelineStateHash, ObjectPoolHandle16>;
		using PipelineBindingLayoutCollection = NativeHandleHashPoolCollection<ManagedPipelineBindingLayoutGraphicObjectHandle,
			PipelineBindingLayoutReference, PipelineBindingLayoutConstReference, PipelineBindingLayoutHash, ObjectPoolHandle16>;

	private:
		ShaderResourceViewPool m_shaderResourceViews;

		GPUBufferPool m_buffers;

		Texture2DPool m_textures2D;

		VertexShaderPool m_vertexShaders;
		PixelShaderPool m_pixelShaders;

		PipelineStateCollection m_pipelineStateCollection;
		PipelineBindingLayoutCollection m_pipelineBindingLayoutCollection;
		InputLayoutCollection m_inputLayoutCollection;


		//Resource deleting
		void deleteShaderResourceViewInternal(IGraphicResourceDescriptor* descriptor);

		void deleteGPUBufferInternal(IGPUBuffer* buffer);

		void deleteTexture2DInternal(ITexture2D* texture);

		void deleteVertexShaderInternal(IVertexShader* shader);
		void deletePixelShaderInternal(IPixelShader* shader);

		void deletePipelineStateInternal(IPipelineState* state);
		void deletePipelineBindingLayoutInternal(IPipelineBindingLayout* bindingLayout);

		//Reference deleting
		void deleteShaderResourceViewReference(ShaderResourceViewHandleID handleID);

		void deleteGPUBufferReference(GPUBufferHandleID handleID);

		void deleteTexture2DReference(TextureHandleID handleID);

		void deleteVertexShaderReference(VertexShaderHandleID handleID);
		void deletePixelShaderReference(PixelShaderHandleID handleID);

		void deleteInputLayoutReference(InputLayoutHandleID handleID);

	public:
		GraphicResourceManager() = default;
		~GraphicResourceManager() { release(); }

		virtual bool init() override;
		virtual void release() override;

		//
		virtual ShaderResourceViewReference createShaderResourceDescriptor(GraphicResourceReference resource) override;

		//Buffer
		virtual GPUBufferReference createGPUBuffer(const GPUBufferDesc& desc, GraphicStateValueType initialState,
			const InitialGPUBufferData* initialData = nullptr) override;
		virtual GPUBufferReference getGPUBuffer(GPUBufferHandleID handle) override;
		virtual bool isValidGPUBuffer(GPUBufferHandleID handle) override;

		virtual ShaderResourceViewReference createBufferResourceDescriptor(GPUBufferReference buffer) override;

		//Texture
		virtual Texture2DReference createTexture2D(const Texture2DDesc& desc) override;
		virtual Texture2DReference getTexture2D(TextureHandleID handle) override;
		virtual bool isValidTexture2D(TextureHandleID handle) const override;

		// Vertex Shader
		virtual VertexShaderReference createVertexShader(const void* code, size_t codeSize) override;
		virtual VertexShaderReference getVertexShader(VertexShaderHandleID handle) override;
		virtual bool isValidVertexShader(VertexShaderHandleID handle) const override;

		// Pixel Shader
		virtual PixelShaderReference createPixelShader(const void* code, size_t codeSize) override;
		virtual PixelShaderReference getPixelShader(PixelShaderHandleID handle) override;
		virtual bool isValidPixelShader(PixelShaderHandleID handle) const override;

		//PipelineState
		virtual PipelineStateReference getPipelineState(const PipelineStateInfo& desc,
			PipelineBindingLayoutConstReference bindingLayout, InputLayoutConstReference inputlayout) override;

		virtual PipelineBindingLayoutReference getPipelineBindingLayout(const PipelineBindingLayoutDesc& desc) override;

		//InputLayout
		virtual InputLayoutReference addInputLayout(const InputLayoutDesc& desc) override;
		virtual InputLayoutReference getInputLayout(InputLayoutHash hash) override;
	};
}
