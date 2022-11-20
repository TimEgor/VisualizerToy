#pragma once

#include "Multithreading/CountableObject/CountableReference.h"

#include <cstdint>
#include <type_traits>

namespace VT
{
	template <typename ResourceType, typename HandleIDType>
	struct GraphicResourceHandle : public CountableObjectBase
	{
		ResourceType* m_resourcePtr = nullptr;
		HandleIDType m_resourceHandle = 0;

		static_assert(std::is_integral<HandleIDType>::value);
	};

	class ITexture2D;
	using Texture2DHandleID = uint32_t;
	using Texture2DResourceHandle = GraphicResourceHandle<ITexture2D, Texture2DHandleID>;

	COUNTABLE_REFERENCES_DECLARATION(Texture2DResourceHandle)
}