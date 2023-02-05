#pragma once

#include "Core/TypeHashMacros.h"
#include "GraphicObject/IGraphicObject.h"
#include "GraphicSynchronization/IFence.h"

namespace VT
{
	using GraphicStateValueType = uint32_t;
	enum CommonGraphicState : GraphicStateValueType
	{
		UNDEFINED = 0,
		SHADER_RESOURCE = 1,
		SHADER_RESOURCE_COMPUTE = 1 << 1,
		UNORDERED_ACCESS = 1 << 2,
		COMMON_READ = 1 << 3,
		COPY_SRC = 1 << 4,
		COPY_DEST = 1 << 5,

		MAX_COMMON_STATE = COPY_DEST
	};

	enum class GraphicResourceUsingMarkType : uint32_t
	{
		GRAPHIC,
		COPY,

		COUNT
	};

	using GraphicResourceType = HashTyped::Type;
	using GraphicResourceTypeHash = uint32_t;

	class IGraphicResource : public IGraphicObject, public HashTyped
	{
	public:
		IGraphicResource() = default;

		virtual bool isUsing(GraphicResourceUsingMarkType markType, FenceValueType fenceVal) const = 0;
		virtual void markUsage(GraphicResourceUsingMarkType markType, FenceValueType fenceVal) = 0;

		virtual GraphicStateValueType getState() const = 0;
		virtual void setState(GraphicStateValueType state) = 0;

		virtual GraphicResourceType getType() const = 0;
	};
}

#define VT_GRAPHIC_RESOURCE_TYPE(RESOURCE_TYPE) VT_HASH_TYPE(#RESOURCE_TYPE, VT::GraphicResourceType, GraphicResource)