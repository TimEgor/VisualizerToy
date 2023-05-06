#pragma once

#include "EntityComponentSystem/EntityComponentSystem.h"

namespace VT
{
	class EntityComponentSystem;
	class IScene;
	class ILevel;
	class DefaultRenderingData;

	class PreparingRenderingDataSystem final
	{
	private:
		static void prepareMeshData(const IScene* scene, const EntityComponentSystem* ecs, DefaultRenderingData& renderingData);
		static void preparePointLightData(const IScene* scene, const EntityComponentSystem* ecs, DefaultRenderingData& renderingData);
		static void prepareDirectionalLightData(const IScene* scene, const EntityComponentSystem* ecs, DefaultRenderingData& renderingData);
		static void prepareCameraData(VT_Entity cameraEntity, const IScene* scene, const EntityComponentSystem* ecs, DefaultRenderingData& renderingData);

	public:
		PreparingRenderingDataSystem() = default;

		static void prepareData(const ILevel& level, DefaultRenderingData& renderingData);
	};
}
