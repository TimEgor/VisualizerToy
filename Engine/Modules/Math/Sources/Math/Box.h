#pragma once

#include "Vector.h"

namespace VT
{
	struct AABB final
	{
		Vector3 m_min;
		Vector3 m_max;

		AABB() = default;
	};
}
