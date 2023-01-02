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
	};
}
