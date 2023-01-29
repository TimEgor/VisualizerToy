#pragma once

#include "Core/HashFunctions/StdHashCombine.h"
#include "GraphicObject/IGraphicObject.h"

#include <cstdint>
#include <vector>

namespace VT
{
	using PipelineBindingLayoutHash = uint32_t;

	enum ShaderStageVisibility : uint8_t
	{
		ALL_STAGES = 0,
		VERTEX_STAGE = 1,
		HULL_STAGE = 2,
		DOMAIN_STAGE = 4,
		GEOMETRY_STAGE = 8,
		PIXEL_STAGE = 16
	};

	struct ConstDescriptorBinding final
	{
		uint8_t m_valuesCount;
		uint8_t m_shaderRegister;
		uint8_t m_shaderRegisterSpace;
		ShaderStageVisibility m_shaderStageVisibility;

		ConstDescriptorBinding(uint8_t valuesCount, uint8_t shaderRegister,
			uint8_t shaderRegisterSpace, ShaderStageVisibility stages)
			: m_valuesCount(valuesCount), m_shaderRegister(shaderRegister),
			m_shaderRegisterSpace(shaderRegisterSpace), m_shaderStageVisibility(stages) {}

		PipelineBindingLayoutHash getHash() const
		{
			PipelineBindingLayoutHash result = 0;

			hashCombine(result, m_valuesCount);
			hashCombine(result, m_shaderRegister);
			hashCombine(result, m_shaderRegisterSpace);
			hashCombine(result, m_shaderStageVisibility);

			return result;
		}
	};

	using ResourceDesriptorBinding = ConstDescriptorBinding;

	struct PipelineBindingLayoutDesc final
	{
		std::vector<ResourceDesriptorBinding> m_descriptorBindings;

		PipelineBindingLayoutHash getHash() const
		{
			PipelineBindingLayoutHash result = 0;

			for (const auto& binding : m_descriptorBindings)
			{
				hashCombine(result, binding.getHash());
			}

			return result;
		}
	};

	class IPipelineBindingLayout : public IGraphicObject
	{
	public:
		IPipelineBindingLayout() = default;
		virtual ~IPipelineBindingLayout() = default;

		virtual PipelineBindingLayoutHash getHash() const = 0;
	};
}
