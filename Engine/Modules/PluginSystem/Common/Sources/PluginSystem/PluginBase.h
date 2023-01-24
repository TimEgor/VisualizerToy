#pragma once

#include "IPlugin.h"

namespace VT
{
	class PluginBase : public IPlugin
	{
	private:
		VT_PLATFORM_HANDLE_MODULE m_handle = 0;
		PluginID m_id = InvalidPluginID;

	public:
		PluginBase(PluginID id, VT_PLATFORM_HANDLE_MODULE nativeHandle)
			: m_handle(nativeHandle), m_id(id) {}

		virtual PluginID getID() const override { return m_id; }
		virtual VT_PLATFORM_HANDLE_MODULE getNativeHandle() const override { return m_handle; }
	};
}