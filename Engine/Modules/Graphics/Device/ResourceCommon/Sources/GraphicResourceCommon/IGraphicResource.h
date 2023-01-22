#pragma once

#include "Core/TypeHashMacros.h"

namespace VT
{
	using GraphicResourceType = HashTyped::Type;
	using GraphicResourceTypeHash = uint32_t;

	class IGraphicResource : public HashTyped
	{
	public:
		IGraphicResource() = default;
		virtual ~IGraphicResource() = default;

		virtual void* getNativeHandle() const = 0;

		template<typename T>
		T* getNativeHandleCast() const { return reinterpret_cast<T*>(getNativeHandle()); }

		virtual GraphicResourceType getType() const = 0;
	};
}

#define VT_GRAPHIC_RESOURCE_TYPE(RESOURCE_TYPE) VT_HASH_TYPE(#RESOURCE_TYPE, VT::GraphicResourceType, GraphicResource)