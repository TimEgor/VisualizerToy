#pragma once

#include "Core/Platform.h"

#include <cstdint>

namespace VT
{
	using PluginID = uint32_t;
	constexpr PluginID InvalidPluginID = 0;

	class PluginBase
	{
	private:
		VT_PLATFORM_HANDLE_MODULE m_handle = 0;
		PluginID m_id = InvalidPluginID;

	public:
		PluginBase(PluginID id, VT_PLATFORM_HANDLE_MODULE nativeHandle)
			: m_handle(nativeHandle), m_id(id) {}
		virtual ~PluginBase() {}

		virtual void onLoaded() = 0;
		virtual void onUnloaded() = 0;

		inline PluginID getID() const { return m_id; };

		inline VT_PLATFORM_HANDLE_MODULE getNativeHandle() const { return m_handle; }
	};
}