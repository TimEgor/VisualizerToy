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

	enum TextureUsage : GraphicStateValueType
	{
		TEXTURE_USAGE_UNKNOWN,

		TEXTURE_USAGE_RENDER_TARGET,
		TEXTURE_USAGE_DEPTH_STENCIL,
		TEXTURE_USAGE_DENY_SHADER_RESOURCE
	};

	enum TextureState
	{
		RENDER_TARGET = MAX_COMMON_STATE << 1,
		DEPTH_STENCIL = MAX_COMMON_STATE << 2,
		DEPTH_STENCIL_READONLY = MAX_COMMON_STATE << 3,
		PRESENTING = MAX_COMMON_STATE << 4
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