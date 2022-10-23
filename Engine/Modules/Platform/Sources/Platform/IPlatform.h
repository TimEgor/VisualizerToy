#pragma once

#include "Core/HashFunctions/CRC32.h"

namespace VT
{
	class IWindowSystem;

	using PlatformType = uint32_t;

	class IPlatform
	{
	public:
		IPlatform() = default;
		virtual ~IPlatform() {}

		virtual IWindowSystem* createWindowSystem() = 0;

		virtual PlatformType getType() const = 0;
	};
}

#define VT_PLATFORM_TYPE(PLATFORM_TYPE)																	\
	static inline constexpr VT::PlatformType getPlatformType() { return VT::crc32(#PLATFORM_TYPE); }	\
	virtual VT::PlatformType getType() const { return getPlatformType(); }