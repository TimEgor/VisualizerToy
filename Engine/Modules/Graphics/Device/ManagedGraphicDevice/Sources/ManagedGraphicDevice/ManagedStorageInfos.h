#pragma once

#include "ManagedGraphicDevice/ManagedObjectStorageInfo.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedBuffer.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedShaderObjects.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedPipelineObjects.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedTextureObjects.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedCommandObjects.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedSyncObjects.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedDescriptorObject.h"

namespace VT::ManagedGraphicDevice
{
	MANAGED_OBJECT_STORAGE_INFO(ManagedGPUBuffer, ManagedGPUBufferBase)

	MANAGED_OBJECT_STORAGE_INFO(ManagedTexture2D, ManagedTexture2DBase)

	MANAGED_OBJECT_STORAGE_INFO(ManagedVertexShader, ManagedVertexShaderBase)
	MANAGED_OBJECT_STORAGE_INFO(ManagedPixelShader, ManagedPixelShaderBase)
	MANAGED_OBJECT_STORAGE_INFO(ManagedComputeShader, ManagedComputeShaderBase)

	MANAGED_OBJECT_STORAGE_INFO(ManagedGraphicPipelineState, ManagedGraphicPipelineStateBase)
	MANAGED_OBJECT_STORAGE_INFO(ManagedComputePipelineState, ManagedComputePipelineStateBase)
	MANAGED_OBJECT_STORAGE_INFO(ManagedPipelineBindingLayout, ManagedPipelineBindingLayoutBase)

	MANAGED_OBJECT_STORAGE_INFO(ManagedCommandList, ManagedCommandListBase)

	MANAGED_OBJECT_STORAGE_INFO(ManagedFence, ManagedFenceBase)

	MANAGED_OBJECT_STORAGE_INFO(ManagedGraphicResourceDescriptor, ManagedGraphicResourceDescriptorBase)
}