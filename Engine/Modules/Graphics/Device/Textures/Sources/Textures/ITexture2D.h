#pragma once

#include "TextureCommon.h"
#include "GraphicResourceCommon/Format.h"

namespace VT
{
	struct Texture2DDesc final
	{
		TextureDimension m_width = 0;
		TextureDimension m_height = 0;
		Format m_format = Format::UNDEFINED;
	};

	class ITexture2D
	{
	public:
		ITexture2D() = default;
		virtual ~ITexture2D() {}

		virtual const Texture2DDesc& getDesc() const = 0;
	};
}