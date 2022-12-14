#pragma once

#include "GraphicResourceManager/IGraphicResourceManager.h"
#include "ObjectPool/ThreadSafeObjectPool.h"
#include "ManagedResourceHandles.h"
#include "ManagedResourceObjectPoolHandle.h"
#include "ResourceSystem/ResourceData.h"

namespace VT
{
	class GraphicResourceManager : public IGraphicResourceManager
	{
		friend ManagedTexture2DResourceHandle;

		friend ManagedVertexShaderResourceHandle;
		friend ManagedPixelShaderResourceHandle;

		using Texture2DPool = ThreadSafeObjectPool<ManagedTexture2DResourceHandle, ObjectPoolHandle32>;

		using VertexShaderPool = ThreadSafeObjectPool<ManagedVertexShaderResourceHandle, ManagedResourceObjectPoolHandle32>;
		using PixelShaderPool = ThreadSafeObjectPool<ManagedPixelShaderResourceHandle, ManagedResourceObjectPoolHandle32>;

	private:
		Texture2DPool m_textures2D;

		VertexShaderPool m_vertexShaders;
		PixelShaderPool m_pixelShaders;

		void deleteTexture2DInternal(ITexture2D* texture);
		void deleteTexture2DReference(Texture2DPool::HandleElementType handle);

		void deleteVertexShaderInternal(IVertexShader* shader);
		void deleteVertexShaderReference(VertexShaderPool::HandleElementType handle);

		void deletePixelShaderInternal(IPixelShader* shader);
		void deletePixelShaderReference(PixelShaderPool::HandleElementType handle);

	public:
		GraphicResourceManager() = default;
		~GraphicResourceManager() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual Texture2DResourceHandleReference createTexture2D(const Texture2DDesc& desc) override;
		virtual Texture2DResourceHandleReference getTexture2D(Texture2DHandleID handle) override;
		virtual bool isValidTexture2D(Texture2DHandleID handle) const override;

		// Vertex Shader
		virtual VertexShaderResourceHandleReference createVertexShader(const void* code, size_t codeSize) override;
		virtual VertexShaderResourceHandleReference getVertexShader(VertexShaderHandleID handle) override;
		virtual bool isValidVertexShader(VertexShaderHandleID handle) const override;

		// Pixel Shader
		virtual PixelShaderResourceHandleReference createPixelShader(const void* code, size_t codeSize) override;
		virtual PixelShaderResourceHandleReference getPixelShader(PixelShaderHandleID handle) override;
		virtual bool isValidPixelShader(PixelShaderHandleID handle) const override;
	};
}
