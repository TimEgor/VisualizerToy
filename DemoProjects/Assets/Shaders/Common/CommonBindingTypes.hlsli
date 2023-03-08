#ifndef COMMON_BINDING_TYPES_HLSL
#define COMMON_BINDING_TYPES_HLSL

struct UniqueBinding
{
	uint m_bindingIndex;
};

struct ElementBinding
{
	uint m_bindingIndex;
	uint m_elementIndex;
};

#endif //COMMON_BINDING_TYPES_HLSL