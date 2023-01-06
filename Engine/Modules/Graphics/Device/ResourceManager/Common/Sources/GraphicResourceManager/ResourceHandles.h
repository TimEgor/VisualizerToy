#pragma once

#include "ResourceHandleDeclaration.h"

#include <cstdint>

namespace VT
{
	GRAPHIC_RESOURCE(GPUBuffer, IGPUBuffer, uint32_t);

	GRAPHIC_RESOURCE(Texture2D, ITexture2D, uint32_t);
	GRAPHIC_RESOURCE_VIEW(Texture2DView, ITexture2DView, Texture2DHandleID, uint32_t);

	GRAPHIC_RESOURCE(VertexShader, IVertexShader, uint32_t);
	GRAPHIC_RESOURCE(PixelShader, IPixelShader, uint32_t);

	GRAPHIC_RESOURCE(PipelineState, IPipelineState, uint32_t);
}