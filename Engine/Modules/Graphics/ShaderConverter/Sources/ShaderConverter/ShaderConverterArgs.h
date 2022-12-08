#pragma once

#include "ResourceConverter/IResourceConverterArgs.h"
#include "Shaders/IShaders.h"

#define VT_SHADER_RESOURCE_CONVERTER_ARGS_TYPE

namespace VT
{
	class ShaderResourceConverterArgs : public IResourceConverterArgs
	{
	private:
		ShaderStageType m_stage;

	public:
		ShaderResourceConverterArgs(ShaderStageType stage)
			: m_stage(stage) {}

		virtual ResourceConverterArgsHash getHash() const override { return static_cast<ResourceConverterArgsHash>(m_stage); }
		ShaderStageType getStageType() const { return m_stage; }

		VT_RESOURCE_CONVERTER_ARGS_TYPE(VT_SHADER_RESOURCE_CONVERTER_ARGS_TYPE)
	};
}