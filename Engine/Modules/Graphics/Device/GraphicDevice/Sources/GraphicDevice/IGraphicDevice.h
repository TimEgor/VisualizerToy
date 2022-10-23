#pragma once

#include "Core/HashFunctions/CRC32.h"

namespace VT
{
	using GraphicDeviceType = uint32_t;

	class IGraphicDevice
	{
	public:
		IGraphicDevice() = default;
		virtual ~IGraphicDevice() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual GraphicDeviceType getType() const = 0;
	};
}

#define VT_GRAPHIC_DEVICE_TYPE(DEVICE_TYPE)																		\
	static inline constexpr VT::GraphicDeviceType getGraphicDeviceType() { return VT::crc32(#DEVICE_TYPE); }	\
	virtual VT::GraphicDeviceType getType() const { return getGraphicDeviceType(); }