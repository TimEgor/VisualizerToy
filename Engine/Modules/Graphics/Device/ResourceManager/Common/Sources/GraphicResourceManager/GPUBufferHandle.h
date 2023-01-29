#pragma once

#include "ResourceHandleDecalaration.h"
#include "GPUBuffer/IGPUBuffer.h"

namespace VT
{
	class IGPUBuffer;

	using GPUBufferHandleID = GraphicResourceHandleID;

	class GPUBufferResourceHandle : public GraphicTypedResourceHandle<IGPUBuffer>
	{
	public:
		GPUBufferResourceHandle() = default;
		GPUBufferResourceHandle(IGPUBuffer* buffer, GraphicResourceHandleID handleID)
			: GraphicTypedResourceHandle(buffer, handleID) {}

		IGPUBuffer* getBuffer() const { return getTypedResource(); }
	};

	COUNTABLE_REFERENCES_DECLARATION_BY_NAME(GPUBufferResourceHandle, GPUBuffer)
}