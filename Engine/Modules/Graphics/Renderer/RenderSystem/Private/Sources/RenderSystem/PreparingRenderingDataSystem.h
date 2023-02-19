#pragma once

namespace VT
{
	class EntityComponentSystem;
	class IScene;
	class ILevel;
	class RenderingData;

	class PreparingRenderingDataSystem final
	{
	private:
		static void prepareMeshData(const IScene* scene, const EntityComponentSystem* ecs, RenderingData& renderingData);
		static void prepareLightData(const IScene* scene, const EntityComponentSystem* ecs, RenderingData& renderingData);

	public:
		PreparingRenderingDataSystem() = default;

		static void prepareData(const ILevel& level, RenderingData& renderingData);
	};
}
