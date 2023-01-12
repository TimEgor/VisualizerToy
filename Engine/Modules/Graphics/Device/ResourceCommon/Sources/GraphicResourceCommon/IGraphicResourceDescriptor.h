#pragma once

#include <cstdint>

namespace VT
{
	using DescriptorBindingHeapOffsetType = uint32_t;

	enum class GraphicResourceDescriptorType
	{
		UNDEFINED,

		RTV,
		DSV,
		SRV
	};

	class IGraphicResourceDescriptor
	{
	public:
		IGraphicResourceDescriptor() = default;
		virtual ~IGraphicResourceDescriptor() = default;

		virtual DescriptorBindingHeapOffsetType getBindingHeapOffset() const = 0;
		virtual bool isValid() const = 0;

		virtual GraphicResourceDescriptorType getType() const = 0;
	};
}
