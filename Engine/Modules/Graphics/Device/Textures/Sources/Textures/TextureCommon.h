#pragma once

#include "GraphicResourceCommon/GraphicResourceBase.h"

#include <cstdint>

#define VT_GRAPHIC_RESOURCE_TYPE_TEXTURE

namespace VT
{
	using TextureDimension = uint32_t;

	enum class TextureDimensionType
	{
		DIMENSION_1D,
		DIMENSION_2D,
		DIMENSION_3D
	};

	enum TextureUsage : GraphicResourceUsageValueType
	{
		TEXTURE_USAGE_RENDER_TARGET = GRAPHIC_MAX_USAGE << 1,
		TEXTURE_USAGE_DEPTH_STENCIL = GRAPHIC_MAX_USAGE << 2
	};

	enum TextureState : GraphicResourceStateValueType
	{
		TEXTURE_STATE_RENDER_TARGET = GRAPHIC_MAX_COMMON_STATE << 1,
		TEXTURE_STATE_DEPTH_STENCIL = GRAPHIC_MAX_COMMON_STATE << 2,
		TEXTURE_STATE_DEPTH_STENCIL_READONLY = GRAPHIC_MAX_COMMON_STATE << 3,
		TEXTURE_STATE_PRESENTING = GRAPHIC_MAX_COMMON_STATE << 4
	};

	class ITexture : public GraphicResourceBase
	{
	public:
		ITexture() = default;
		virtual ~ITexture() {}

		virtual TextureDimensionType getDimension() const = 0;

		VT_GRAPHIC_RESOURCE_TYPE(VT_GRAPHIC_RESOURCE_TYPE_TEXTURE);
	};
}