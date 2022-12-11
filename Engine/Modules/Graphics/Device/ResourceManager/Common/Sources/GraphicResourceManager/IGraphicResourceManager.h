#pragma once

#include <functional>

#include "ResourceHandles.h"

namespace VT
{
	class FileName;
	class IWindow;

	struct SwapChainDesc;
	struct Texture2DDesc;

	class IGraphicResourceManager
	{
	public:
		using OnLoadedVertexShaderCallback = std::function<void(VertexShaderResourceHandleReference)>;
		using OnLoadedPixelShaderCallback = std::function<void(PixelShaderResourceHandleReference)>;

	public:
		IGraphicResourceManager() = default;
		virtual ~IGraphicResourceManager() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		//Texture2D
		virtual Texture2DResourceHandleReference createTexture2D(const Texture2DDesc& desc) = 0;
		virtual Texture2DResourceHandleReference getTexture2D(Texture2DHandleID handle) = 0;
		virtual bool isValidTexture2D(Texture2DHandleID handle) const = 0;

		//Vertex shader
		virtual VertexShaderResourceHandleReference createVertexShader(const void* data, size_t dataSize) = 0;
		virtual VertexShaderResourceHandleReference getVertexShader(VertexShaderHandleID handle) = 0;
		virtual bool isValidVertexShader(VertexShaderHandleID handle) const = 0;

		virtual VertexShaderResourceHandleReference loadVertexShader(const FileName& shaderPath) = 0;
		virtual VertexShaderResourceHandleReference loadVertexShaderAsync(const FileName& shaderPath,
			OnLoadedVertexShaderCallback callback = nullptr) = 0;

		//Pixel shader
		virtual PixelShaderResourceHandleReference createPixelShader(const void* data, size_t dataSize) = 0;
		virtual PixelShaderResourceHandleReference getPixelShader(PixelShaderHandleID handle) = 0;
		virtual bool isValidPixelShader(PixelShaderHandleID handle) const = 0;

		virtual PixelShaderResourceHandleReference loadPixelShader(const FileName& shaderPath) = 0;
		virtual PixelShaderResourceHandleReference loadPixelShaderAsync(const FileName& shaderPath,
			OnLoadedPixelShaderCallback callback = nullptr) = 0;
	};
}