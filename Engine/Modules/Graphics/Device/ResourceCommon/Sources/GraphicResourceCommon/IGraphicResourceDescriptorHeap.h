#pragma once

#include "IGraphicResourceDescriptor.h"
#include "GraphicObject/IGraphicObject.h"

namespace VT
{
	struct GraphicResourceDescriptorHeapDesc final
	{
		DescriptorBindingHeapOffsetType m_elementsCount = 0;
		GraphicResourceDescriptorType m_descriptorType = GraphicResourceDescriptorType::UNDEFINED;
		bool m_isShaderVisible = true;
	};

	class IGraphicResourceDescriptorHeap : public IGraphicObject
	{
	public:
		IGraphicResourceDescriptorHeap() = default;
		virtual ~IGraphicResourceDescriptorHeap() {}

		virtual DescriptorBindingHeapOffsetType allocateDescriptor() = 0;
		virtual void deallocateDescriptor(DescriptorBindingHeapOffsetType offset) = 0;

		virtual const GraphicResourceDescriptorHeapDesc& getDesc() const = 0;
	};
}
