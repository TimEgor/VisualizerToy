#pragma once

#include "Math/Vector.h"

namespace VT
{
	struct DirectionalLightComponent final
	{
		Vector3 m_color = Vector3Identity;
		float m_intensity = 1.0f;
	};
}
