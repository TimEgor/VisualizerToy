#pragma once

#include "Core/TypeHashMacros.h"
#include "MonitorInfo.h"

namespace VT
{
	class IInputSystem;

	class IWindow;
	class IWindowEventSystem;

	using PlatformType = HashTyped::Type;

	class IPlatform : public HashTyped
	{
	public:
		IPlatform() = default;
		virtual ~IPlatform() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual void* getNativeHandle() const = 0;

		//WindowSystem
		virtual IWindow* createWindow() = 0;
		virtual IWindowEventSystem* createWindowEventSystem() = 0;

		virtual IInputSystem* createInputSystem() = 0;

		virtual uint32_t getMonitorCount() const = 0;
		virtual const MonitorInfo& getMonitorInfo(uint32_t index) const = 0;

		virtual PlatformType getType() const = 0;
	};
}

#define VT_PLATFORM_TYPE(PLATFORM_TYPE) VT_HASH_TYPE(#PLATFORM_TYPE, VT::PlatformType, Platform)