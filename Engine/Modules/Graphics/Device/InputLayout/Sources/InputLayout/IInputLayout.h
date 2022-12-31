#pragma once

#include "Core/HashFunctions/StdHashCombine.h"

#include <cstdint>
#include <vector>

namespace VT
{
	using InputLayoutElementNameHash = uint32_t;
	using InputLayoutElementHash = uint32_t;

	enum InputLayoutElementType
	{
		INT8,
		INT16,
		INT32,
		UINT8,
		UINT16,
		UINT32,
		FLOAT16,
		FLOAT32,

		UNKNOWN
	};

	using InputLayoutElementSizeType = uint32_t;

	constexpr InputLayoutElementSizeType INPUT_LAYOUT_ELEMENT_TYPE_INT8_SIZE = sizeof(int8_t);
	constexpr InputLayoutElementSizeType INPUT_LAYOUT_ELEMENT_TYPE_INT16_SIZE = sizeof(int16_t);
	constexpr InputLayoutElementSizeType INPUT_LAYOUT_ELEMENT_TYPE_INT32_SIZE = sizeof(int32_t);
	constexpr InputLayoutElementSizeType INPUT_LAYOUT_ELEMENT_TYPE_UINT8_SIZE = sizeof(uint8_t);
	constexpr InputLayoutElementSizeType INPUT_LAYOUT_ELEMENT_TYPE_UINT16_SIZE = sizeof(uint16_t);
	constexpr InputLayoutElementSizeType INPUT_LAYOUT_ELEMENT_TYPE_UINT32_SIZE = sizeof(uint32_t);
	constexpr InputLayoutElementSizeType INPUT_LAYOUT_ELEMENT_TYPE_FLOAT16_SIZE = sizeof(uint16_t);
	constexpr InputLayoutElementSizeType INPUT_LAYOUT_ELEMENT_TYPE_FLOAT32_SIZE = sizeof(float);

	InputLayoutElementSizeType getInputLayoutElementTypeSize(InputLayoutElementType type);

	using InputLayoutElementHash = uint32_t;

	struct InputLayoutElementDesc final
	{
		InputLayoutElementNameHash m_nameHash = 0;
		uint32_t m_index = 0;
		uint32_t m_slot = 0;
		InputLayoutElementType m_type;
		uint32_t m_componentsNum = 0;
		uint32_t m_offset = 0;
		uint32_t m_stride = 0;

		InputLayoutElementHash getHash() const
		{
			InputLayoutElementHash result = 0;

			hashCombine(result, m_nameHash);
			hashCombine(result, m_index);
			hashCombine(result, m_slot);
			hashCombine(result, m_type);
			hashCombine(result, m_componentsNum);
			hashCombine(result, m_offset);
			hashCombine(result, m_stride);

			return result;
		}
	};

	using InputLayoutHash = uint32_t;

	struct InputLayoutDesc final
	{
		using ElementDescContainer = std::vector<InputLayoutElementDesc>;

		ElementDescContainer m_elements;

		InputLayoutHash getHash() const
		{
			InputLayoutHash result = 0;

			for (const auto& element : m_elements)
			{
				hashCombine(result, element.getHash());
			}

			return result;
		}
	};
}
