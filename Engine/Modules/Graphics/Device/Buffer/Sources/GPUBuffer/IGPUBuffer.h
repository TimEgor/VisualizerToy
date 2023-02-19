#pragma once

#include "GraphicResourceCommon/GraphicResourceBase.h"

#define VT_GRAPHIC_RESOURCE_TYPE_BUFFER

namespace VT
{
	enum class GPUBufferUsageType
	{
		UNDEFINED = 0,

		INDEX_BUFFER = 1,
		VERTEX_BUFFER = 2
	};

	enum class GPUBufferFlag
	{
		NONE,

		STRUCTURED,
		RAW
	};

	enum GPUBufferState : GraphicStateValueType
	{
		VERTEX_BUFFER = MAX_COMMON_STATE << 1,
		INDEX_BUFFER = MAX_COMMON_STATE << 2,
		CONSTANT_BUFFER = MAX_COMMON_STATE << 3,
		INDIRECT_ARGUMENT = MAX_COMMON_STATE << 4
	};

	struct GPUBufferDesc final
	{
		uint32_t m_byteSize = 0;
		uint32_t m_byteStride = 0;
		GPUBufferFlag m_flag = GPUBufferFlag::NONE;
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
