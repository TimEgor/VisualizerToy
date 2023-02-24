#pragma once

#include "ResourceConverter/IResourceConverterArgs.h"
#include "Shaders/IShaders.h"

#define VT_SHADER_RESOURCE_CONVERTER_ARGS_TYPE

namespace VT
{
	class ShaderResourceConverterArgs : public IResourceConverterArgs
	{
	private:
		ShaderType m_stage;

	public:
		ShaderResourceConverterArgs(ShaderType stage)
			: m_stage(stage) {}

		virtual ResourceConverterArgsHash getHash() const override { return static_cast<ResourceConverterArgsHash>(m_stage); }
		ShaderType getStageType() const { return m_stage; }

		VT_RESOURCE_CONVERTER_ARGS_TYPE(VT_SHADER_RESOURCE_CONVERTER_ARGS_TYPE)
	};
}