#pragma once

#include "ConverterCommon/ResourceConverterPlugin.h"
#include "ModelConverterFBX/ModelConverterFBX.h"

namespace VT_MODEL_RC
{
	class ModelConverterFBXPlugin final : public VT_RC::ResourceConverterPluginBase
	{
	public:
		ModelConverterFBXPlugin(VT::PluginID id, VT_PLATFORM_HANDLE_MODULE nativeHandle)
			: VT_RC::ResourceConverterPluginBase(id, nativeHandle) {}

		virtual VT::IFileResourceConverter* createConverter() override;
		virtual VT::ResourceConverterType getConverterType() const override { return VT_MODEL_RC::ModelConverterFBX::getResourceConverterType(); }
	};
}