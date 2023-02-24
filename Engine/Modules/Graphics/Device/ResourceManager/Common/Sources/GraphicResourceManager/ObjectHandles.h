#pragma once

#include "TextureHandles.h"
#include "InputLayoutHandle.h"

#include "GPUBuffer/IGPUBuffer.h"
#include "Shaders/IShaders.h"
#include "GraphicPipeline/IPipelineBindingLayout.h"
#include "GraphicPipeline/IPipelineState.h"

#include <cstdint>

namespace VT
{
	using GPUBufferHandleID = GraphicResourceHandleID;

	using ShaderResourceViewHandleID = uint32_t;
	using RenderTargetViewHandleID = uint16_t;
	using DepthStencilViewHandleID = uint16_t;

	using VertexShaderHandleID = uint32_t;
	using PixelShaderHandleID = uint32_t;
	using ComputeShaderHandleID = uint32_t;

	using PipelineStateHandleID = uint16_t;
	using PipelineBindingLayoutHandleID = uint16_t;

	GRAPHIC_RESOURCE_VIEW(Shader, ShaderResourceView, ShaderResourceViewHandleID);
	GRAPHIC_RESOURCE_VIEW(RenderTarget, RenderTargetView, RenderTargetViewHandleID);
	GRAPHIC_RESOURCE_VIEW(DepthStencil, DepthStencilView, DepthStencilViewHandleID);

	GRAPHIC_TYPED_RESOURCE(GPUBuffer, IGPUBuffer);

	TYPED_TEXTURE_RESOURCE(Texture2D, ITexture2D);

	GRAPHIC_TYPED_OBJECT(VertexShader, IVertexShader, VertexShaderHandleID);
	GRAPHIC_TYPED_OBJECT(PixelShader, IPixelShader, PixelShaderHandleID);
	GRAPHIC_TYPED_OBJECT(ComputeShader, IComputeShader, ComputeShaderHandleID);

	GRAPHIC_TYPED_OBJECT(GraphicPipelineState, IGraphicPipelineState, PipelineStateHandleID);
	GRAPHIC_TYPED_OBJECT(ComputePipelineState, IComputePipelineState, PipelineStateHandleID);
	GRAPHIC_TYPED_OBJECT(PipelineBindingLayout, IPipelineBindingLayout, PipelineBindingLayoutHandleID);
}