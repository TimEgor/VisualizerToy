#pragma once

#include "Core/HashFunctions/CRC32.h"

namespace VT
{
	using GraphicPlatformType = uint32_t;

	class IGraphicDevice;

	class IGraphicPlatform
	{
	public:
		IGraphicPlatform() = default;
		virtual ~IGraphicPlatform() {}

		virtual bool init(bool isSwapChainEnabled) = 0;
		virtual void release() = 0;

		virtual IGraphicDevice* createGraphicDevice() = 0;

		virtual GraphicPlatformType getType() const = 0;
	};
}

#define VT_GRAPHIC_PLATFORM_TYPE(PLATFORM_TYPE)																		\
	static inline constexpr VT::GraphicPlatformType getGraphicPlatformType() { return VT::crc32(#PLATFORM_TYPE); }	\
	virtual VT::GraphicPlatformType getType() const override { return getGraphicPlatformType(); }