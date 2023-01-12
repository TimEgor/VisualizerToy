#pragma once

#include "IGraphicResourceDescriptorHeap.h"

namespace VT
{
	class GraphicResourceDescriptorHeapBase : public IGraphicResourceDescriptorHeap
	{
	private:
		const GraphicResourceDescriptorHeapDesc m_desc;

	public:
		GraphicResourceDescriptorHeapBase(const GraphicResourceDescriptorHeapDesc& desc)
			: m_desc(desc) {}

		virtual const GraphicResourceDescriptorHeapDesc& getDesc() const override { return m_desc; }
	};
}
