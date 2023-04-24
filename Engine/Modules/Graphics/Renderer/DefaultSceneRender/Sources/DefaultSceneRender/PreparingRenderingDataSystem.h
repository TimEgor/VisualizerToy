#pragma once

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
		static void prepareLightData(const IScene* scene, const EntityComponentSystem* ecs, DefaultRenderingData& renderingData);

	public:
		PreparingRenderingDataSystem() = default;

		static void prepareData(const ILevel& level, DefaultRenderingData& renderingData);
	};
}
