#pragma once

#include "Core/HashedType.h"
#include "GraphicObject/IGraphicObject.h"
#include "GraphicSynchronization/IFence.h"
#include "Format.h"

namespace VT
{
	using GraphicResourceUsageValueType = uint32_t;
	enum CommonGraphicResourceUsage : GraphicResourceUsageValueType
	{
		GRAPHIC_USAGE_UNDEFINED = 0,

		GRAPHIC_USAGE_ALLOW_UNORDERED_ACCESS = 1,
		GRAPHIC_USAGE_SHADER_RESOURCE = 1 << 1,

		GRAPHIC_MAX_USAGE = GRAPHIC_USAGE_SHADER_RESOURCE
	};

	using GraphicResourceStateValueType = uint32_t;
	enum CommonGraphicResourceState : GraphicResourceStateValueType
	{
		GRAPHIC_STATE_UNDEFINED = 0,

		GRAPHIC_STATE_SHADER_RESOURCE = 1,
		GRAPHIC_STATE_SHADER_RESOURCE_COMPUTE = 1 << 1,
		GRAPHIC_STATE_UNORDERED_ACCESS = 1 << 2,
		GRAPHIC_STATE_COMMON_READ = 1 << 3,
		GRAPHIC_STATE_COPY_SRC = 1 << 4,
		GRAPHIC_STATE_COPY_DEST = 1 << 5,

		GRAPHIC_MAX_COMMON_STATE = GRAPHIC_STATE_COPY_DEST
	};

	enum class GraphicResourceUsingMarkType : uint32_t
	{
		GRAPHIC,
		COPY,

		COUNT
	};

	using GraphicResourceType = HashedType::Type;
	using GraphicResourceTypeHash = uint32_t;

	class IGraphicResource : public IGraphicObject, public HashedType
	{
	public:
		IGraphicResource() = default;

		virtual bool isUsing(GraphicResourceUsingMarkType markType, FenceValueType fenceVal) const = 0;
		virtual void markUsage(GraphicResourceUsingMarkType markType, FenceValueType fenceVal) = 0;

		virtual GraphicResourceStateValueType getState() const = 0;
		virtual void setState(GraphicResourceStateValueType state) = 0;

		virtual Format getFormat() const = 0;
		virtual GraphicResourceUsageValueType getUsage() const = 0;
		virtual bool getHostVisibleState() const = 0;

		virtual GraphicResourceType getType() const = 0;
	};
}

#define VT_GRAPHIC_RESOURCE_TYPE(RESOURCE_TYPE) VT_HASH_TYPE(#RESOURCE_TYPE, VT::GraphicResourceType, GraphicResource)