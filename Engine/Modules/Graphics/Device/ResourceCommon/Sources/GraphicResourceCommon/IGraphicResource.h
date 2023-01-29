#pragma once

#include "Core/TypeHashMacros.h"
#include "GraphicObject/IGraphicObject.h"

namespace VT
{
	using GraphicResourceType = HashTyped::Type;
	using GraphicResourceTypeHash = uint32_t;

	class IGraphicResource : public IGraphicObject, public HashTyped
	{
	public:
		IGraphicResource() = default;

		virtual GraphicResourceType getType() const = 0;
	};
}

#define VT_GRAPHIC_RESOURCE_TYPE(RESOURCE_TYPE) VT_HASH_TYPE(#RESOURCE_TYPE, VT::GraphicResourceType, GraphicResource)