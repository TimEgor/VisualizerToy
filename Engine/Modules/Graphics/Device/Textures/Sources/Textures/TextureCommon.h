#pragma once

#include "GraphicResourceCommon/IGraphicResource.h"

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

	class ITexture : public IGraphicResource
	{
	public:
		ITexture() = default;
		virtual ~ITexture() {}

		virtual TextureDimensionType getDimension() const = 0;

		VT_GRAPHIC_RESOURCE_TYPE(VT_GRAPHIC_RESOURCE_TYPE_TEXTURE);
	};
}