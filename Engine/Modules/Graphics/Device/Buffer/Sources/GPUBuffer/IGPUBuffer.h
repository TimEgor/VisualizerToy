#pragma once

#include "GraphicResourceCommon/IGraphicResource.h"

#define VT_GRAPHIC_RESOURCE_TYPE_BUFFER

namespace VT
{
	enum class GPUBufferUsageType
	{
		UNDEFINED = 0,

		INDEX_BUFFER = 1,
		VERTEX_BUFFER = 2
	};

	struct GPUBufferDesc final
	{
		uint32_t m_byteSize = 0;
		GPUBufferUsageType m_usage = GPUBufferUsageType::UNDEFINED;
	};

	class IGPUBuffer : public IGraphicResource
	{
	public:
		IGPUBuffer() = default;

		virtual void mapData(void** data) = 0;
		virtual void unmapData() = 0;

		virtual const GPUBufferDesc& getDesc() const = 0;

		VT_GRAPHIC_RESOURCE_TYPE(VT_GRAPHIC_RESOURCE_TYPE_BUFFER);
	};
}
