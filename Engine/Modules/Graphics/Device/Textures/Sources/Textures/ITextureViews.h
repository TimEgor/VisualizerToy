#pragma once

#include "GraphicResourceCommon/Format.h"

#include <cstdint>

namespace VT
{
	enum class TextureViewAspect
	{
		COLOR,
		DEPTH,
		STENCIL
	};

	struct TextureViewDesc final
	{
		TextureViewAspect m_aspect;
		uint32_t m_baseMipLevel;
		uint32_t m_mipLevelCount;
		uint32_t m_baseArrayLayer;
		uint32_t m_arrayLayerCount;
	};

	class ITexture2DView
	{
	public:
		ITexture2DView() = default;
		virtual ~ITexture2DView() {}

		virtual const TextureViewDesc& getDesc() const = 0;
	};
}
