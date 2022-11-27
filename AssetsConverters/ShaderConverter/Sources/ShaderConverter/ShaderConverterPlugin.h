#pragma once

#include "PluginSystem/ExternalPluginCore.h"

namespace VT_DUMMY_RS
{
	class DummyResourceSystemPlugin final : public VT::PluginBase
	{
	public:
		DummyResourceSystemPlugin(VT::PluginID id, VT_PLATFORM_HANDLE_MODULE nativeHandle)
			: VT::PluginBase(id, nativeHandle) {}

		virtual void onLoaded() override;
		virtual void onUnloaded() override;
	};
}