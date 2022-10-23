#pragma once

#include "Core/Platform.h"
#include <cstdint>

namespace VT
{
	using PluginID = uint32_t;
	constexpr PluginID InvalidPluginID = 0;

	class IPlugin
	{
	public:
		IPlugin() = default;
		virtual ~IPlugin() {}

		virtual void onLoaded() = 0;
		virtual void onUnloaded() = 0;

		virtual PluginID getID() const = 0;
		virtual VT_PLATFORM_HANDLE_MODULE getNativeHandle() const = 0;
	};
}