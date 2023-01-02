#pragma once

#include "GPUBuffer/GPUBufferBase.h"
#include "ManagedGraphicDevice/ManagedGraphicObject.h"

namespace VT::ManagedGraphicDevice
{
	using ManagedGPUBufferHandle = ObjectPoolHandle32;
	using ManagedGPUBufferObject = ManagedGraphicObject<ManagedGPUBufferHandle>;

	class ManagedGPUBufferBase : public GPUBufferBase, public ManagedGPUBufferObject
	{
	public:
		ManagedGPUBufferBase(const GPUBufferDesc& desc)
			: GPUBufferBase(desc) {}
	};
}