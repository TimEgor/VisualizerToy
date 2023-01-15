#include "InputLayout.h"

#include <cassert>

namespace VT
{
	struct InputLayoutDescInfo final
	{
		uint32_t m_inputElementsCount = 0;
		uint32_t m_inputBindingsCount = 0;
	};

	struct InputLayoutElementDescInfo final
	{
		uint32_t m_index = 0;
		uint32_t m_slot = 0;
		uint32_t m_offset = 0;
		uint32_t m_componentNum = 0;
		InputLayoutElementType m_type = InputLayoutElementType::UNDEFINED;

		uint32_t m_semanticNameLength = 0;
	};
}

VT::InputLayoutElementSizeType VT::getInputLayoutElementTypeSize(InputLayoutElementType type)
{
	switch (type)
	{
	case InputLayoutElementType::INT8:
		return INPUT_LAYOUT_ELEMENT_TYPE_INT8_SIZE;
	case InputLayoutElementType::INT16:
		return INPUT_LAYOUT_ELEMENT_TYPE_INT16_SIZE;
	case InputLayoutElementType::INT32:
		return INPUT_LAYOUT_ELEMENT_TYPE_INT32_SIZE;
	case InputLayoutElementType::UINT8:
		return INPUT_LAYOUT_ELEMENT_TYPE_UINT8_SIZE;
	case InputLayoutElementType::UINT16:
		return INPUT_LAYOUT_ELEMENT_TYPE_UINT16_SIZE;
	case InputLayoutElementType::UINT32: 
		return INPUT_LAYOUT_ELEMENT_TYPE_UINT32_SIZE;
	case InputLayoutElementType::FLOAT16: 
		return INPUT_LAYOUT_ELEMENT_TYPE_FLOAT16_SIZE;
	case InputLayoutElementType::FLOAT32:
		return INPUT_LAYOUT_ELEMENT_TYPE_FLOAT32_SIZE;
	default:
		return 0;
	}
}

void VT::InputLayoutDesc::serialize(const InputLayoutDesc& desc, void** data, size_t& dataSize)
{
	const size_t elementsCount = desc.m_elements.size();
	const size_t bindingsCount = desc.m_bindings.size();

	const size_t infoMemSize = sizeof(InputLayoutDescInfo);
	const size_t bindingsMemSize = bindingsCount * sizeof(InputLayoutBindingDesc);

	size_t elementsMemSize = elementsCount * sizeof(InputLayoutElementDescInfo);
	for (const auto& elementDesc : desc.m_elements)
	{
		elementsMemSize += elementDesc.m_semanticName.size();
	}

	const size_t totalMemSize = infoMemSize + elementsMemSize + bindingsMemSize;

	uint8_t* mem = new uint8_t[totalMemSize];
	dataSize = totalMemSize;

	InputLayoutDescInfo* descInfo = reinterpret_cast<InputLayoutDescInfo*>(mem);

	descInfo->m_inputElementsCount = static_cast<uint32_t>(elementsCount);
	descInfo->m_inputBindingsCount = static_cast<uint32_t>(bindingsCount);

	uint8_t* currentElementInfo = mem + infoMemSize;
	for (const auto& elementDesc : desc.m_elements)
	{
		InputLayoutElementDescInfo* elementDescInfo = reinterpret_cast<InputLayoutElementDescInfo*>(currentElementInfo);

		const size_t semanticNameLength = elementDesc.m_semanticName.size();

		elementDescInfo->m_index = elementDesc.m_index;
		elementDescInfo->m_slot = elementDesc.m_slot;
		elementDescInfo->m_offset = elementDesc.m_offset;
		elementDescInfo->m_componentNum = elementDesc.m_componentsCount;
		elementDescInfo->m_type = elementDesc.m_type;
		elementDescInfo->m_semanticNameLength = static_cast<uint32_t>(semanticNameLength);

		currentElementInfo += sizeof(InputLayoutElementDescInfo);
		
		memcpy(currentElementInfo, elementDesc.m_semanticName.data(), semanticNameLength);

		currentElementInfo += semanticNameLength;
	}

	InputLayoutBindingDesc* bindings = reinterpret_cast<InputLayoutBindingDesc*>(currentElementInfo);
	memcpy(bindings, desc.m_bindings.data(), bindingsMemSize);

	*data = mem;
}

void VT::InputLayoutDesc::deserialize(InputLayoutDesc& desc, const void* data, size_t dataSize)
{
	const uint8_t* memData = reinterpret_cast<const uint8_t*>(data);

	const InputLayoutDescInfo* descInfo = reinterpret_cast<const InputLayoutDescInfo*>(data);

	const size_t infoMemSize = sizeof(InputLayoutDescInfo);
	const size_t bindingMemSize = descInfo->m_inputBindingsCount * sizeof(InputLayoutBindingDesc);

	const InputLayoutElementDesc* elements = reinterpret_cast<const InputLayoutElementDesc*>(memData + infoMemSize);

	desc.m_elements.resize(descInfo->m_inputElementsCount);
	desc.m_bindings.resize(descInfo->m_inputBindingsCount);

	const uint8_t* currentElementInfo = memData + infoMemSize;
	for (auto& elementDesc : desc.m_elements)
	{
		const InputLayoutElementDescInfo* elementDescInfo = reinterpret_cast<const InputLayoutElementDescInfo*>(currentElementInfo);
		const char* semanticName = reinterpret_cast<const char*>(currentElementInfo + sizeof(InputLayoutElementDescInfo));

		elementDesc.m_semanticName = semanticName;
		elementDesc.m_index = elementDescInfo->m_index;
		elementDesc.m_slot = elementDescInfo->m_slot;
		elementDesc.m_offset = elementDescInfo->m_offset;
		elementDesc.m_componentsCount = elementDescInfo->m_componentNum;
		elementDesc.m_type = elementDescInfo->m_type;

		currentElementInfo += sizeof(InputLayoutElementDescInfo) + elementDescInfo->m_semanticNameLength;
	}

	const InputLayoutBindingDesc* bindings = reinterpret_cast<const InputLayoutBindingDesc*>(currentElementInfo);
	memcpy(desc.m_bindings.data(), bindings, bindingMemSize);
}
