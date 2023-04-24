#pragma once

#include "Math/Vector.h"

namespace VT
{
	struct PointLightData final
	{
		Vector3 m_color = Vector3Identity;
		float m_radius = 0.0f;
		Vector3 m_position = Vector3Zero;

		PointLightData() = default;
		PointLightData(const Vector3& color, float radius, const Vector3& position)
			: m_color(color), m_radius(radius), m_position(position) {}
	};
}
