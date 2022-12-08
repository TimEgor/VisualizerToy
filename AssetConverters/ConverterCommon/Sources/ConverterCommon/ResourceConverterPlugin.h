#pragma once

#include "PluginSystem/ExternalPluginCore.h"
#include "ResourceConverter/IResourceConverter.h"

namespace VT_RC
{
	class ResourceConverterPluginBase : public VT::PluginBase
	{
	public:
		ResourceConverterPluginBase(VT::PluginID id, VT_PLATFORM_HANDLE_MODULE nativeHandle)
			: VT::PluginBase(id, nativeHandle) {}

		virtual void onLoaded() override;
		virtual void onUnloaded() override;

		virtual VT::IFileResourceConverter* createConverter() = 0;
		virtual VT::ResourceConverterType getConverterType() const = 0;
	};
}