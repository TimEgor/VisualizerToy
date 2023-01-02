#pragma once

#include "ManagedGraphicDevice/ManagedObjectStorageInfo.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedBuffer.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedShaderObjects.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedPipelineStateObject.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedTexture2DObject.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedSwapChainObject.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedCommandObjects.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedSyncObjects.h"

namespace VT::ManagedGraphicDevice
{
	MANAGED_OBJECT_STORAGE_INFO(ManagedSwapChain, ManagedSwapChainBase)

	MANAGED_OBJECT_STORAGE_INFO(ManagedGPUBuffer, ManagedGPUBufferBase)

	MANAGED_OBJECT_STORAGE_INFO(ManagedTexture2D, ManagedTexture2DBase)
	MANAGED_OBJECT_STORAGE_INFO(ManagedTexture2DView, ManagedTexture2DViewBase)

	MANAGED_OBJECT_STORAGE_INFO(ManagedVertexShader, ManagedVertexShaderBase)
	MANAGED_OBJECT_STORAGE_INFO(ManagedPixelShader, ManagedPixelShaderBase)

	MANAGED_OBJECT_STORAGE_INFO(ManagedPipelineState, ManagedPipelineStateBase)

	MANAGED_OBJECT_STORAGE_INFO(ManagedCommandPool, ManagedCommandPoolBase)
	MANAGED_OBJECT_STORAGE_INFO(ManagedCommandList, ManagedCommandListBase)

	MANAGED_OBJECT_STORAGE_INFO(ManagedFence, ManagedFenceBase)
	MANAGED_OBJECT_STORAGE_INFO(ManagedSemaphore, ManagedSemaphoreBase)
}