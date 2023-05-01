#pragma once

#include "Math/Transform.h"

#include <cstdint>

namespace VT
{
	using NodeID = uint32_t;
	constexpr NodeID InvalidNodeID = 0;

	struct NodeTransforms final
	{
		Transform m_localTransform;
		Transform m_worldTransform;
	};
}
