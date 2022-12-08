#pragma once

#include "ConverterCommon/ResourceConverterPlugin.h"
#include "ShaderConverterHLSL.h"

namespace VT_SHADER_RC
{
	class ShaderConverterHLSLPlugin final : public VT_RC::ResourceConverterPluginBase
	{
	public:
		ShaderConverterHLSLPlugin(VT::PluginID id, VT_PLATFORM_HANDLE_MODULE nativeHandle)
			: VT_RC::ResourceConverterPluginBase(id, nativeHandle) {}

		virtual VT::IFileResourceConverter* createConverter() override;
		virtual VT::ResourceConverterType getConverterType() const override { return VT_SHADER_RC::ShaderConverterHLSL::getResourceConverterType(); }

		ShaderConverterHLSL* createShaderConverterHLSL(ShaderConverterHLSL::OutputILType ilType);
	};
}