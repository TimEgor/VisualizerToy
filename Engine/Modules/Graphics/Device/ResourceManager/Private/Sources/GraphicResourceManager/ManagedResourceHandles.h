#pragma once

#include "ManagedResourceHandleDeclartion.h"
#include "GraphicResourceManager/ResourceHandles.h"

namespace VT
{
	MANAGED_GRAPHIC_RESOURCE(Texture2DResourceHandle)
	MANAGED_GRAPHIC_RESOURCE_VIEW(Texture2DViewResourceHandle)

	MANAGED_GRAPHIC_RESOURCE(VertexShaderResourceHandle)
	MANAGED_GRAPHIC_RESOURCE(PixelShaderResourceHandle)

	MANAGED_GRAPHIC_RESOURCE(PipelineStateResourceHandle)

	NAMED_MANAGED_GRAPHIC_RESOURCE(VertexShaderResourceHandle)
	NAMED_MANAGED_GRAPHIC_RESOURCE(PixelShaderResourceHandle)
}