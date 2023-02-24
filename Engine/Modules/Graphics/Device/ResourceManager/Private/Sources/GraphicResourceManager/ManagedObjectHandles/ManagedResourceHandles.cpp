#include "ManagedResourceHandles.h"

#include "ManagedResourceHandleReleaseDeclaration.h"

namespace VT
{
	MANAGED_GRAPHIC_OBJECT_VIEW_DESTROY_RELEASE_IMPL(ShaderGraphicResourceViewHandle, deleteShaderResourceViewInternal, deleteShaderResourceViewReference)
	MANAGED_GRAPHIC_OBJECT_VIEW_DESTROY_RELEASE_IMPL(RenderTargetGraphicResourceViewHandle, deleteRenderTargetViewInternal, deleteRenderTargetViewReference)

	MANAGED_GRAPHIC_OBJECT_DESTROY_RELEASE_IMPL(GPUBufferGraphicResourceHandle, deleteGPUBufferInternal, deleteGPUBufferReference)

	MANAGED_GRAPHIC_OBJECT_DESTROY_RELEASE_IMPL(Texture2DResourceHandle, deleteTexture2DInternal, deleteTexture2DReference)

	MANAGED_GRAPHIC_OBJECT_DESTROY_RELEASE_IMPL(VertexShaderGraphicObjectHandle, deleteVertexShaderInternal, deleteVertexShaderReference)
	MANAGED_GRAPHIC_OBJECT_DESTROY_RELEASE_IMPL(PixelShaderGraphicObjectHandle, deletePixelShaderInternal, deletePixelShaderReference)
	MANAGED_GRAPHIC_OBJECT_DESTROY_RELEASE_IMPL(ComputeShaderGraphicObjectHandle, deleteComputeShaderInternal, deleteComputeShaderReference)

	MANAGED_GRAPHIC_OBJECT_RELEASE_IMPL(GraphicPipelineStateGraphicObjectHandle, deleteGraphicPipelineStateInternal)
	MANAGED_GRAPHIC_OBJECT_RELEASE_IMPL(ComputePipelineStateGraphicObjectHandle, deleteComputePipelineStateInternal)
	MANAGED_GRAPHIC_OBJECT_RELEASE_IMPL(PipelineBindingLayoutGraphicObjectHandle, deletePipelineBindingLayoutInternal)

	NAMED_MANAGED_GRAPHIC_OBJECT_DESTROY_IMPL(VertexShaderGraphicObjectHandle, deleteVertexShaderReference)
	NAMED_MANAGED_GRAPHIC_OBJECT_DESTROY_IMPL(PixelShaderGraphicObjectHandle, deletePixelShaderReference)
	NAMED_MANAGED_GRAPHIC_OBJECT_DESTROY_IMPL(ComputeShaderGraphicObjectHandle, deleteComputeShaderReference)
}