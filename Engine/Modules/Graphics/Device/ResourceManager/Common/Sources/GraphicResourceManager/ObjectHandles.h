#pragma once

#include "GPUBufferHandle.h"
#include "TextureHandles.h"

#include "Shaders/IShaders.h"
#include "GraphicPipeline/IPipelineBindingLayout.h"
#include "GraphicPipeline/IPipelineState.h"

#include <cstdint>

namespace VT
{
	TYPED_TEXTURE_RESOURCE(Texture2D, ITexture2D);

	using VertexShaderHandleID = uint32_t;
	GRAPHIC_TYPED_OBJECT(VertexShader, IVertexShader, VertexShaderHandleID);
	using PixelShaderHandleID = uint32_t;
	GRAPHIC_TYPED_OBJECT(PixelShader, IPixelShader, PixelShaderHandleID);

	GRAPHIC_TYPED_OBJECT(PipelineState, IPipelineState, uint16_t);
	GRAPHIC_TYPED_OBJECT(PipelineBindingLayout, IPipelineBindingLayout, uint16_t);
}