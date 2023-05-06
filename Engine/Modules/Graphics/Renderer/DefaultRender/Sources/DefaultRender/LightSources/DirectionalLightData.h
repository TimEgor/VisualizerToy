#pragma once

#include "Math/Vector.h"

namespace VT
{
	struct DirectionalLightData final
	{
		Vector3 m_color = Vector3Identity;
		float m_intensity = 0.0f;
		Vector3 m_dir = Vector3Zero;

		DirectionalLightData() = default;
		DirectionalLightData(const Vector3& color, float intensity, const Vector3& dir)
			: m_color(color), m_intensity(intensity), m_dir(dir) {}
	};
}
