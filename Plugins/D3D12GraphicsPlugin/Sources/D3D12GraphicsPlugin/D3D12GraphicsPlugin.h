#pragma once

#include "PluginSystem/ExternalPluginCore.h"

namespace VT_D3D12
{
	class D3D12GraphicPlugin final : public VT::PluginBase
	{
	public:
		D3D12GraphicPlugin(VT::PluginID id, VT_PLATFORM_HANDLE_MODULE nativeHandle)
			: VT::PluginBase(id, nativeHandle) {}

		virtual void onLoaded() override;
		virtual void onUnloaded() override;
	};
}