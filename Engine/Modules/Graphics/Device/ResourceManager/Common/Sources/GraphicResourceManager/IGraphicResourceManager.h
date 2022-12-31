#pragma once

#include <functional>

#include "ResourceHandles.h"
#include "InputLayoutHandle.h"

class IRenderPass;

namespace VT
{
	struct TextureViewDesc;
	class IWindow;
	class IRenderPass;

	struct SwapChainDesc;
	struct Texture2DDesc;
	struct PipelineStateInfo;

	class IGraphicResourceManager
	{
	public:
		using OnLoadedVertexShaderCallback = std::function<void(VertexShaderReference)>;
		using OnLoadedPixelShaderCallback = std::function<void(PixelShaderReference)>;

	public:
		IGraphicResourceManager() = default;
		virtual ~IGraphicResourceManager() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		//Texture2D
		virtual Texture2DReference createTexture2D(const Texture2DDesc& desc) = 0;
		virtual Texture2DReference getTexture2D(Texture2DHandleID handle) = 0;
		virtual bool isValidTexture2D(Texture2DHandleID handle) const = 0;

		virtual Texture2DViewReference createTexture2DView(Texture2DReference texture, const TextureViewDesc& desc) = 0;
		virtual Texture2DViewReference getTexture2DView(Texture2DViewHandleID handle) = 0;
		virtual bool isValidTexture2DView(Texture2DViewHandleID handle) = 0;

		//Vertex shader
		virtual VertexShaderReference createVertexShader(const void* data, size_t dataSize) = 0;
		virtual VertexShaderReference getVertexShader(VertexShaderHandleID handle) = 0;
		virtual VertexShaderReference getNamedVertexShader(GraphicResourceNameID handle) = 0;
		virtual VertexShaderReference getNamedVertexShader(const GraphicResourceName& name) = 0;
		virtual bool isValidVertexShader(VertexShaderHandleID handle) const = 0;
		virtual bool isValidNamedVertexShader(GraphicResourceNameID handle) const = 0;

		virtual VertexShaderReference loadVertexShader(const FileName& shaderPath) = 0;
		virtual VertexShaderReference loadVertexShaderAsync(const FileName& shaderPath,
			OnLoadedVertexShaderCallback callback = nullptr) = 0;

		//Pixel shader
		virtual PixelShaderReference createPixelShader(const void* data, size_t dataSize) = 0;
		virtual PixelShaderReference getPixelShader(PixelShaderHandleID handle) = 0;
		virtual PixelShaderReference getNamedPixelShader(GraphicResourceNameID handle) = 0;
		virtual PixelShaderReference getNamedPixelShader(const GraphicResourceName& name) = 0;
		virtual bool isValidPixelShader(PixelShaderHandleID handle) const = 0;
		virtual bool isValidNamedPixelShader(GraphicResourceNameID handle) const = 0;

		virtual PixelShaderReference loadPixelShader(const FileName& shaderPath) = 0;
		virtual PixelShaderReference loadPixelShaderAsync(const FileName& shaderPath,
			OnLoadedPixelShaderCallback callback = nullptr) = 0;

		//PipelineState
		virtual PipelineStateReference getPipelineState(const PipelineStateInfo& desc) = 0;

		//InputLayout
		virtual void addInputLayout(const InputLayoutDesc& desc) = 0;
		virtual InputLayoutReference getInputLayout(InputLayoutHash hash) = 0;
	};
}
