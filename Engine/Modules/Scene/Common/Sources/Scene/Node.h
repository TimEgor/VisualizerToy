#pragma once

#include "Math/Matrix.h"

#include <cstdint>

namespace VT
{
	using NodeID = uint32_t;
	constexpr NodeID InvalidNodeID = 0;


	struct NodeTransforms final
	{
		Matrix44 m_localTransform = Matrix44Identity;
		Matrix44 m_globalTransform = Matrix44Identity;
	};
}
