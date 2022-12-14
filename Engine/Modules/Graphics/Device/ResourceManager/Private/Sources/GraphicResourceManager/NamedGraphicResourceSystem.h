#pragma once

#include "GraphicResourceManager.h"
#include "NamedGraphicResourcePool.h"

namespace VT
{
	class NamedGraphicResourceSystem final : public GraphicResourceManager
	{
		friend NamedManagedVertexShaderResourceHandle;
		friend NamedManagedPixelShaderResourceHandle;

		using NamedVertexShaderPool = NamedGraphicResourcePool<NamedManagedVertexShaderResourceHandle,
		VertexShaderResourceHandleReference, ManagedResourceObjectPoolHandle32>;
		using NamedPixelShaderPool = NamedGraphicResourcePool<NamedManagedPixelShaderResourceHandle,
		PixelShaderResourceHandleReference, ManagedResourceObjectPoolHandle32>;

	private:
		NamedVertexShaderPool m_namedVertexPool;
		NamedPixelShaderPool m_namedPixelPool;

		void deleteVertexShaderReference(FileNameID nameID);
		void deletePixelShaderReference(FileNameID nameID);

	public:
		NamedGraphicResourceSystem() = default;

		virtual bool init() override;
		virtual void release() override;

		// Vertex Shader
		virtual VertexShaderResourceHandleReference getVertexShader(VertexShaderHandleID handle) override;
		virtual bool isValidVertexShader(VertexShaderHandleID handle) const override;

		virtual VertexShaderResourceHandleReference getNamedVertexShader(FileNameID handle) override;
		virtual bool isValidNamedVertexShader(FileNameID handle) const override;

		virtual VertexShaderResourceHandleReference loadVertexShader(const FileName& shaderPath) override;
		virtual VertexShaderResourceHandleReference loadVertexShaderAsync(const FileName& shaderPath,
			OnLoadedVertexShaderCallback callback) override;

		// Pixel Shader
		virtual PixelShaderResourceHandleReference getPixelShader(PixelShaderHandleID handle) override;
		virtual bool isValidPixelShader(PixelShaderHandleID handle) const override;

		virtual PixelShaderResourceHandleReference getNamedPixelShader(FileNameID handle) override;
		virtual bool isValidNamedPixelShader(FileNameID handle) const override;

		virtual PixelShaderResourceHandleReference loadPixelShader(const FileName& shaderPath) override;
		virtual PixelShaderResourceHandleReference loadPixelShaderAsync(const FileName& shaderPath,
			OnLoadedPixelShaderCallback callback) override;
	};
}