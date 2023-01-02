#pragma once

#include "GraphicResourceManager/IGraphicResourceManager.h"
#include "ObjectPool/ThreadSafeObjectPool.h"
#include "ManagedResourceHandles.h"
#include "NamedReferencePool/NamedObjectPoolHandle.h"
#include "InputLayoutCollection.h"
#include "PipelineStateCollection.h"

namespace VT
{
	class GraphicResourceManager : public IGraphicResourceManager
	{
		friend ManagedGPUBufferResourceHandle;

		friend ManagedTexture2DResourceHandle;
		friend ManagedTexture2DViewResourceHandle;

		friend ManagedVertexShaderResourceHandle;
		friend ManagedPixelShaderResourceHandle;

		friend ManagedPipelineStateResourceHandle;

		friend class ManagedInputLayoutHandle;

		using GPUBufferPool = ThreadSafeObjectPool<ManagedGPUBufferResourceHandle, ObjectPoolHandle32>;

		using Texture2DPool = ThreadSafeObjectPool<ManagedTexture2DResourceHandle, NamedObjectPoolHandle32>;
		using Texture2DViewPool = ThreadSafeObjectPool<ManagedTexture2DViewResourceHandle, NamedObjectPoolHandle32>;

		using VertexShaderPool = ThreadSafeObjectPool<ManagedVertexShaderResourceHandle, NamedObjectPoolHandle32>;
		using PixelShaderPool = ThreadSafeObjectPool<ManagedPixelShaderResourceHandle, NamedObjectPoolHandle32>;

	private:
		GPUBufferPool m_buffers;

		Texture2DPool m_textures2D;
		Texture2DViewPool m_texture2DViews;

		VertexShaderPool m_vertexShaders;
		PixelShaderPool m_pixelShaders;

		PipelineStateCollection m_pipelineStateCollection;
		InputLayoutCollection m_inputLayoutCollection;

		//Resource deleting
		void deleteGPUBufferInternal(IGPUBuffer* buffer);

		void deleteTexture2DInternal(ITexture2D* texture);
		void deleteTexture2DViewInternal(ITexture2DView* view);

		void deleteVertexShaderInternal(IVertexShader* shader);
		void deletePixelShaderInternal(IPixelShader* shader);

		void deletePipelineStateInternal(IPipelineState* state);

		//Reference deleting
		void deleteGPUBufferReference(GPUBufferHandleID handleID);

		void deleteTexture2DReference(Texture2DHandleID handleID);
		void deleteTexture2DViewReference(Texture2DViewHandleID handleID);

		void deleteVertexShaderReference(VertexShaderHandleID handleID);
		void deletePixelShaderReference(PixelShaderHandleID handleID);

		void deleteInputLayoutReference(InputLayoutHandleID handleID);

	public:
		GraphicResourceManager() = default;
		~GraphicResourceManager() { release(); }

		virtual bool init() override;
		virtual void release() override;

		//Buffer
		virtual GPUBufferReference createGPUBuffer(const GPUBufferDesc& desc) override;
		virtual GPUBufferReference getGPUBuffer(GPUBufferHandleID handle) override;
		virtual bool isValidGPUBuffer(GPUBufferHandleID handle) override;

		//Texture
		virtual Texture2DReference createTexture2D(const Texture2DDesc& desc) override;
		virtual Texture2DReference getTexture2D(Texture2DHandleID handle) override;
		virtual bool isValidTexture2D(Texture2DHandleID handle) const override;

		virtual Texture2DViewReference createTexture2DView(Texture2DReference texture, const TextureViewDesc& desc) override;
		virtual Texture2DViewReference getTexture2DView(Texture2DViewHandleID handle) override;
		virtual bool isValidTexture2DView(Texture2DViewHandleID handle) override;

		// Vertex Shader
		virtual VertexShaderReference createVertexShader(const void* code, size_t codeSize) override;
		virtual VertexShaderReference getVertexShader(VertexShaderHandleID handle) override;
		virtual bool isValidVertexShader(VertexShaderHandleID handle) const override;

		// Pixel Shader
		virtual PixelShaderReference createPixelShader(const void* code, size_t codeSize) override;
		virtual PixelShaderReference getPixelShader(PixelShaderHandleID handle) override;
		virtual bool isValidPixelShader(PixelShaderHandleID handle) const override;

		//PipelineState
		virtual PipelineStateReference getPipelineState(const PipelineStateInfo& desc, InputLayoutConstReference inputlayout) override;

		//InputLayout
		virtual InputLayoutReference addInputLayout(const InputLayoutDesc& desc) override;
		virtual InputLayoutReference getInputLayout(InputLayoutHash hash) override;
	};
}
