#pragma once

#include "Core/HashFunctions/CRC32.h"

namespace VT
{
	class IWindowContainer;
	class IWindowEventSystem;

	using PlatformType = uint32_t;

	class IPlatform
	{
	public:
		IPlatform() = default;
		virtual ~IPlatform() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual void* getNativeHandle() const = 0;

		//WindowSystem
		virtual IWindowContainer* createWindowContainer() = 0;
		virtual IWindowEventSystem* createWindowEventSystem() = 0;

		virtual PlatformType getType() const = 0;
	};
}

#define VT_PLATFORM_TYPE(PLATFORM_TYPE)																	\
	static inline constexpr VT::PlatformType getPlatformType() { return VT::crc32(#PLATFORM_TYPE); }	\
	virtual VT::PlatformType getType() const { return getPlatformType(); }