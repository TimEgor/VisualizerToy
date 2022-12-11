#pragma once

#include "Multithreading/CountableObject/CountableReference.h"

#include <cstdint>
#include <type_traits>

namespace VT
{
	template <typename ResourceType, typename HandleIDType>
	struct GraphicResourceHandle : public CountableObjectBase
	{
		using BaseResourceType = ResourceType;
		using HandleType = HandleIDType;

		ResourceType* m_resourcePtr = nullptr;
		HandleIDType m_resourceHandle = 0;

		GraphicResourceHandle() = default;
		GraphicResourceHandle(ResourceType* resource, HandleIDType handle)
			: m_resourcePtr(resource), m_resourceHandle(handle) {}

		static_assert(std::is_integral<HandleIDType>::value);
	};

#define GRAPHIC_RESOURCE(NAME, INTERFACE_TYPE, HANDLE_ID_TYPE)							\
	class INTERFACE_TYPE;																\
	using NAME##HandleID = HANDLE_ID_TYPE;												\
	using NAME##ResourceHandle = GraphicResourceHandle<INTERFACE_TYPE, NAME##HandleID>;	\
	COUNTABLE_REFERENCES_DECLARATION(NAME##ResourceHandle)

	GRAPHIC_RESOURCE(Texture2D, ITexture2D, uint32_t);

	GRAPHIC_RESOURCE(VertexShader, IVertexShader, uint32_t);
	GRAPHIC_RESOURCE(PixelShader, IPixelShader, uint32_t);
}