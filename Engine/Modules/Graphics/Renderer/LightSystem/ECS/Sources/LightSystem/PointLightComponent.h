#pragma once

#include "Math/Vector.h"

namespace VT
{
	struct PointLightComponent final
	{
		Vector3 m_color = Vector3Zero;
		float m_radius = 0.0f;
	};
}
