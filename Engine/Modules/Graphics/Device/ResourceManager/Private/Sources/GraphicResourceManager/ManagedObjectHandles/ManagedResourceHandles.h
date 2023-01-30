#pragma once

#include "GraphicResourceManager/ObjectHandles.h"
#include "ManagedResourceHandleDeclartion.h"

namespace VT
{
	MANAGED_GRAPHIC_VIEW_OBJECT(ShaderGraphicResourceViewHandle)
	MANAGED_GRAPHIC_VIEW_OBJECT(RenderTargetGraphicResourceViewHandle)

	MANAGED_GRAPHIC_OBJECT(GPUBufferGraphicResourceHandle);

	MANAGED_GRAPHIC_OBJECT(Texture2DResourceHandle);

	MANAGED_GRAPHIC_OBJECT(VertexShaderGraphicObjectHandle);
	MANAGED_GRAPHIC_OBJECT(PixelShaderGraphicObjectHandle);

	MANAGED_GRAPHIC_OBJECT(PipelineStateGraphicObjectHandle);
	MANAGED_GRAPHIC_OBJECT(PipelineBindingLayoutGraphicObjectHandle);

	NAMED_MANAGED_GRAPHIC_OBJECT(VertexShaderGraphicObjectHandle);
	NAMED_MANAGED_GRAPHIC_OBJECT(PixelShaderGraphicObjectHandle);
}