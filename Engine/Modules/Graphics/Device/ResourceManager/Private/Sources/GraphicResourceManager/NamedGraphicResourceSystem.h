#pragma once

#include "GraphicResourceManager.h"
#include "NamedReferencePool/NamedObjectPool.h"

namespace VT
{
	class NamedGraphicResourceSystem final : public GraphicResourceManager
	{
		friend NamedManagedVertexShaderResourceHandle;
		friend NamedManagedPixelShaderResourceHandle;

		using NamedVertexShaderPool = NamedObjectPool<NamedManagedVertexShaderResourceHandle,
		VertexShaderReference, NamedObjectPoolHandle32>;
		using NamedPixelShaderPool = NamedObjectPool<NamedManagedPixelShaderResourceHandle,
		PixelShaderReference, NamedObjectPoolHandle32>;

	private:
		NamedVertexShaderPool m_namedVertexPool;
		NamedPixelShaderPool m_namedPixelPool;

		void deleteVertexShaderReference(GraphicResourceNameID nameID);
		void deletePixelShaderReference(GraphicResourceNameID nameID);

	public:
		NamedGraphicResourceSystem() = default;

		virtual bool init() override;
		virtual void release() override;

		// Vertex Shader
		virtual VertexShaderReference getVertexShader(VertexShaderHandleID handle) override;
		virtual bool isValidVertexShader(VertexShaderHandleID handle) const override;

		virtual VertexShaderReference getNamedVertexShader(const GraphicResourceName& name) override;
		virtual VertexShaderReference getNamedVertexShader(GraphicResourceNameID handle) override;
		virtual bool isValidNamedVertexShader(GraphicResourceNameID handle) const override;

		virtual VertexShaderReference loadVertexShader(const GraphicResourceName& shaderPath) override;
		virtual VertexShaderReference loadVertexShaderAsync(const GraphicResourceName& shaderPath,
			OnLoadedVertexShaderCallback callback) override;

		// Pixel Shader
		virtual PixelShaderReference getPixelShader(PixelShaderHandleID handle) override;
		virtual bool isValidPixelShader(PixelShaderHandleID handle) const override;

		virtual PixelShaderReference getNamedPixelShader(const GraphicResourceName& name) override;
		virtual PixelShaderReference getNamedPixelShader(GraphicResourceNameID handle) override;
		virtual bool isValidNamedPixelShader(GraphicResourceNameID handle) const override;

		virtual PixelShaderReference loadPixelShader(const GraphicResourceName& shaderPath) override;
		virtual PixelShaderReference loadPixelShaderAsync(const GraphicResourceName& shaderPath,
			OnLoadedPixelShaderCallback callback) override;
	};
}