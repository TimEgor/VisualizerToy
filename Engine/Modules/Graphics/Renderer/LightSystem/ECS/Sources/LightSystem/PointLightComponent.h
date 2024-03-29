#pragma once

#include "Math/Vector.h"

namespace VT
{
	struct PointLightComponent final
	{
		Vector3 m_color = Vector3Identity;
		float m_radius = 0.0f;
		float m_intensity = 1.0f;
	};
}
