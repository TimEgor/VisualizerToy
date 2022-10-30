#pragma once

#include "TextureCommon.h"

namespace VT
{
	struct Texture2DDesc final
	{
		TextureDimension m_width = 0;
		TextureDimension m_height = 0;
	};

	class ITexture2D
	{
	public:
		ITexture2D() = default;
		virtual ~ITexture2D() {}

		virtual const Texture2DDesc& getDesc() const = 0;
	};
}