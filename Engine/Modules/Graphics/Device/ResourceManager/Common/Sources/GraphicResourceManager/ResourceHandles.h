#pragma once

#include <cstdint>
#include <type_traits>

namespace VT
{
	template <typename ResourceType, typename HandleType>
	struct GraphicResourceInfo final
	{
		ResourceType* m_resourcePtr = nullptr;
		HandleType m_resourceHandle = 0;

		static_assert(std::is_integral<HandleType>::value);
	};

	class ITexture2D;
	using Texture2DHandle = uint32_t;
	using Texture2DResourceInfo = GraphicResourceInfo<ITexture2D, Texture2DHandle>;
}