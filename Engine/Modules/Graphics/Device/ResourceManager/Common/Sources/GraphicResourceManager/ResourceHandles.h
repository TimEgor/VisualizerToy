#pragma once

#include "ResourceHandleDeclaration.h"

#include <cstdint>

namespace VT
{
	GRAPHIC_RESOURCE(GPUBuffer, IGPUBuffer, uint32_t);

	GRAPHIC_RESOURCE(Texture2D, ITexture2D, uint32_t);
	GRAPHIC_RESOURCE_VIEW(Texture2DView, IGraphicResourceDescriptor, Texture2DHandleID, uint32_t);

	GRAPHIC_RESOURCE(VertexShader, IVertexShader, uint32_t);
	GRAPHIC_RESOURCE(PixelShader, IPixelShader, uint32_t);

	GRAPHIC_RESOURCE(PipelineState, IPipelineState, uint16_t);
	GRAPHIC_RESOURCE(PipelineBindingLayout, IPipelineBindingLayout, uint16_t);
}