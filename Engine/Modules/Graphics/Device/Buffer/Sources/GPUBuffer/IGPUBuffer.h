#pragma once

#include "GraphicResourceCommon/GraphicResourceBase.h"

#define VT_GRAPHIC_RESOURCE_TYPE_BUFFER

namespace VT
{
	enum class GPUBufferFlag
	{
		NONE,

		STRUCTURED,
		RAW
	};

	enum GPUBufferState : GraphicResourceStateValueType
	{
		GPU_BUFFER_STATE_VERTEX_BUFFER = GRAPHIC_MAX_COMMON_STATE << 1,
		GPU_BUFFER_STATE_INDEX_BUFFER = GRAPHIC_MAX_COMMON_STATE << 2,
		GPU_BUFFER_STATE_CONSTANT_BUFFER = GRAPHIC_MAX_COMMON_STATE << 3,
		GPU_BUFFER_STATE_INDIRECT_ARGUMENT = GRAPHIC_MAX_COMMON_STATE << 4
	};

	struct GPUBufferDesc final
	{
		uint32_t m_byteSize = 0;
		uint32_t m_byteStride = 0;
		GPUBufferFlag m_flag = GPUBufferFlag::NONE;
		GraphicResourceUsageValueType m_usage = GRAPHIC_USAGE_UNDEFINED;
		bool isHostVisible = false;
	};

	struct InitialGPUBufferData final
	{
		const void* data = nullptr;
		size_t dataSize = 0;
	};

	class IGPUBuffer : public GraphicResourceBase
	{
	public:
		IGPUBuffer() = default;

		virtual void mapData(void** data) = 0;
		virtual void unmapData() = 0;

		virtual const GPUBufferDesc& getDesc() const = 0;

		VT_GRAPHIC_RESOURCE_TYPE(VT_GRAPHIC_RESOURCE_TYPE_BUFFER);
	};
}
