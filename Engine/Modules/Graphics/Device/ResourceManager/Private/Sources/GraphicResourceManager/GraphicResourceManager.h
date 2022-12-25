#pragma once

#include "GraphicResourceManager/IGraphicResourceManager.h"
#include "ObjectPool/ThreadSafeObjectPool.h"
#include "ManagedResourceHandles.h"
#include "NamedReferencePool/NamedObjectPoolHandle.h"
#include "PipelineStateCollection.h"

namespace VT
{
	class GraphicResourceManager : public IGraphicResourceManager
	{
		friend ManagedTexture2DResourceHandle;
		friend ManagedTexture2DViewResourceHandle;

		friend ManagedVertexShaderResourceHandle;
		friend ManagedPixelShaderResourceHandle;

		friend ManagedPipelineStateResourceHandle;

		using Texture2DPool = ThreadSafeObjectPool<ManagedTexture2DResourceHandle, NamedObjectPoolHandle32>;
		using Texture2DViewPool = ThreadSafeObjectPool<ManagedTexture2DViewResourceHandle, NamedObjectPoolHandle32>;

		using VertexShaderPool = ThreadSafeObjectPool<ManagedVertexShaderResourceHandle, NamedObjectPoolHandle32>;
		using PixelShaderPool = ThreadSafeObjectPool<ManagedPixelShaderResourceHandle, NamedObjectPoolHandle32>;

	private:
		Texture2DPool m_textures2D;
		Texture2DViewPool m_texture2DViews;

		VertexShaderPool m_vertexShaders;
		PixelShaderPool m_pixelShaders;

		PipelineStateCollection m_pipelineStateCollection;

		//Resource deleting
		void deleteTexture2DInternal(ITexture2D* texture);
		void deleteTexture2DViewInternal(ITexture2DView* view);

		void deleteVertexShaderInternal(IVertexShader* shader);
		void deletePixelShaderInternal(IPixelShader* shader);

		void deletePipelineStateInternal(IPipelineState* state);

		//Reference deleting
		void deleteTexture2DReference(Texture2DPool::Handle handle);
		void deleteTexture2DViewReference(Texture2DViewPool::Handle handle);

		void deleteVertexShaderReference(VertexShaderPool::Handle handle);
		void deletePixelShaderReference(PixelShaderPool::Handle handle);

	public:
		GraphicResourceManager() = default;
		~GraphicResourceManager() { release(); }

		virtual bool init() override;
		virtual void release() override;

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
		virtual PipelineStateReference getPipelineState(const PipelineStateInfo& desc) override;
		virtual bool isValidPipelineState(PipelineStateHandleID handle) const override;
	};
}
