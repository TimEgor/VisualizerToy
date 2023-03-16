#pragma once

#include "IGPUBuffer.h"

namespace VT
{
	class GPUBufferBase : public IGPUBuffer
	{
	private:
		GPUBufferDesc m_desc;

	public:
		GPUBufferBase(const GPUBufferDesc& desc)
			: m_desc(desc) {}

		virtual const GPUBufferDesc& getDesc() const override { return m_desc; }

		virtual Format getFormat() const { return Format::UNDEFINED; }
		virtual GraphicResourceUsageValueType getUsage() const { return m_desc.m_usage; }
		virtual bool getHostVisibleState() const { return m_desc.isHostVisible; }
	};
}
