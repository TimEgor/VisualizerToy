#pragma once

#include "EntityComponentSystem/EntityComponentSystem.h"

namespace VT_DEMO_COMMON
{
	class FreePerspectiveCameraController final
	{
	public:
		static void updateCamera(VT::VT_Entity cameraEntity, float deltaTime);
	};
}