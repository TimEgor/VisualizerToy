#include "InputLayout.h"

#include <cassert>

namespace VT
{
	struct InputLayoutDescInfo final
	{
		uint32_t m_inputElementsCount = 0;
		uint32_t m_inputBindingsCount = 0;
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
	const size_t elementsMemSize = elementsCount * sizeof(InputLayoutElementDesc);
	const size_t bindingsMemSize = bindingsCount * sizeof(InputLayoutBindingDesc);

	const size_t totalMemSize = infoMemSize + elementsMemSize + bindingsMemSize;

	uint8_t* mem = new uint8_t[totalMemSize];
	dataSize = totalMemSize;

	InputLayoutDescInfo* descInfo = reinterpret_cast<InputLayoutDescInfo*>(mem);
	InputLayoutElementDesc* elements = reinterpret_cast<InputLayoutElementDesc*>(mem + infoMemSize);
	InputLayoutBindingDesc* bindings = reinterpret_cast<InputLayoutBindingDesc*>(mem + infoMemSize + elementsMemSize);

	descInfo->m_inputElementsCount = static_cast<uint32_t>(elementsCount);
	descInfo->m_inputBindingsCount = static_cast<uint32_t>(bindingsCount);

	memcpy(elements, desc.m_elements.data(), elementsMemSize);
	memcpy(bindings, desc.m_bindings.data(), bindingsMemSize);

	*data = mem;
}

void VT::InputLayoutDesc::deserialize(InputLayoutDesc& desc, const void* data, size_t dataSize)
{
	const uint8_t* memData = reinterpret_cast<const uint8_t*>(data);

	const InputLayoutDescInfo* descInfo = reinterpret_cast<const InputLayoutDescInfo*>(data);

	const size_t infoMemSize = sizeof(InputLayoutDescInfo);
	const size_t elementsMemSize = descInfo->m_inputElementsCount * sizeof(InputLayoutElementDesc);
	const size_t bindingMemSize = descInfo->m_inputBindingsCount * sizeof(InputLayoutBindingDesc);

	assert(infoMemSize + elementsMemSize + bindingMemSize == dataSize);

	const InputLayoutElementDesc* elements = reinterpret_cast<const InputLayoutElementDesc*>(memData + infoMemSize);
	const InputLayoutBindingDesc* bindings = reinterpret_cast<const InputLayoutBindingDesc*>(memData + infoMemSize + elementsMemSize);

	desc.m_elements.resize(descInfo->m_inputElementsCount);
	desc.m_bindings.resize(descInfo->m_inputBindingsCount);

	memcpy(desc.m_elements.data(), elements, elementsMemSize);
	memcpy(desc.m_bindings.data(), bindings, bindingMemSize);
}
