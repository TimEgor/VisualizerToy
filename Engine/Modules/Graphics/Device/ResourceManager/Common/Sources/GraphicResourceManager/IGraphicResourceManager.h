#pragma once

#include <functional>

#include "Core/FileName/FileName.h"
#include "ObjectHandles.h"

class IRenderPass;

namespace VT
{
	class IWindow;

	struct SwapChainDesc;
	struct GPUBufferDesc;
	struct Texture2DDesc;
	struct GraphicPipelineStateInfo;
	struct PipelineBindingLayoutDesc;

	using GraphicResourceName = FileName;
	using GraphicResourceNameID = FileNameID;

	class IGraphicResourceManager
	{
	public:
		using OnLoadedVertexShaderCallback = std::function<void(VertexShaderReference)>;
		using OnLoadedPixelShaderCallback = std::function<void(PixelShaderReference)>;
		using OnLoadedComputeShaderCallback = std::function<void(ComputeShaderReference)>;

	public:
		IGraphicResourceManager() = default;
		virtual ~IGraphicResourceManager() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		//
		virtual ShaderResourceViewReference createShaderResourceDescriptor(GraphicResourceReference resource) = 0;
		virtual ShaderResourceViewReference createUnorderedAccessResourceDescriptor(GraphicResourceReference resource) = 0;

		virtual RenderTargetViewReference createRenderTargetDescriptor(TextureReference texture) = 0;
		virtual DepthStencilViewReference createDepthStencilDescriptor(TextureReference texture) = 0;

		//Buffers
		virtual GPUBufferReference createGPUBuffer(const GPUBufferDesc& desc, GraphicResourceStateValueType initialState,
			const InitialGPUBufferData* initialData = nullptr) = 0;
		virtual GPUBufferReference getGPUBuffer(GPUBufferHandleID handle) = 0;
		virtual bool isValidGPUBuffer(GPUBufferHandleID handle) = 0;

		virtual ShaderResourceViewReference createBufferResourceDescriptor(GPUBufferReference buffer) = 0;

		//Texture2D
		virtual Texture2DReference createTexture2D(const Texture2DDesc& desc, GraphicResourceStateValueType initialState) = 0;
		virtual Texture2DReference getTexture2D(TextureHandleID handle) = 0;
		virtual bool isValidTexture2D(TextureHandleID handle) const = 0;

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

		//Compute shader
		virtual ComputeShaderReference createComputeShader(const void* data, size_t dataSize) = 0;
		virtual ComputeShaderReference getComputeShader(ComputeShaderHandleID handle) = 0;
		virtual ComputeShaderReference getNamedComputeShader(GraphicResourceNameID handle) = 0;
		virtual ComputeShaderReference getNamedComputeShader(const GraphicResourceName& name) = 0;
		virtual bool isValidComputeShader(ComputeShaderHandleID handle) const = 0;
		virtual bool isValidNamedComputeShader(GraphicResourceNameID handle) const = 0;

		virtual ComputeShaderReference loadComputeShader(const FileName& shaderPath) = 0;
		virtual ComputeShaderReference loadComputeShaderAsync(const FileName& shaderPath,
			OnLoadedComputeShaderCallback callback = nullptr) = 0;

		//Pipeline
		virtual GraphicPipelineStateReference getGraphicPipelineState(const GraphicPipelineStateInfo& desc,
			PipelineBindingLayoutConstReference bindingLayout, InputLayoutConstReference inputlayout) = 0;
		virtual ComputePipelineStateReference getComputePipelineState(const ComputePipelineStateInfo& desc,
			PipelineBindingLayoutConstReference bindingLayout) = 0;

		virtual PipelineBindingLayoutReference getPipelineBindingLayout(const PipelineBindingLayoutDesc& desc) = 0;

		//InputLayout
		virtual InputLayoutReference addInputLayout(const InputLayoutDesc& desc) = 0;
		virtual InputLayoutReference getInputLayout(InputLayoutHash hash) = 0;
	};
}
