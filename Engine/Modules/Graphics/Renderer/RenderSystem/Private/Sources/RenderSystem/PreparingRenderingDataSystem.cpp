#include "PreparingRenderingDataSystem.h"

#include "RenderingData.h"
#include "Scene/IScene.h"
#include "LevelSystem/ILevel.h"
#include "EntityComponentSystem/EntityComponentSystem.h"

#include "Scene/SceneNodeIDComponent.h"
#include "MeshSystem/MeshComponent.h"
#include "LightSystem/PointLightComponent.h"

void VT::PreparingRenderingDataSystem::prepareMeshData(const IScene* scene, const EntityComponentSystem* ecs, RenderingData& renderingData)
{
	auto ecsView = ecs->getView<SceneNodeIDComponent, MeshComponent>();

	for (const auto& [entity, sceneNodeIDComponent, meshComponent] : ecsView.each())
	{
		if (!meshComponent.m_mesh)
		{
			continue;
		}

		const NodeTransforms* nodeTransforms = scene->getNodeTransforms(sceneNodeIDComponent.getNodeID());
		assert(nodeTransforms);

		renderingData.addMesh(meshComponent.m_mesh, nodeTransforms->m_globalTransform.m_matrix);
	}
}

void VT::PreparingRenderingDataSystem::prepareLightData(const IScene* scene, const EntityComponentSystem* ecs, RenderingData& renderingData)
{
	auto ecsView = ecs->getView<SceneNodeIDComponent, PointLightComponent>();

	for (const auto& [entity, sceneNodeIDComponent, pointLightComponent] : ecsView.each())
	{
		const NodeTransforms* nodeTransforms = scene->getNodeTransforms(sceneNodeIDComponent.getNodeID());
		assert(nodeTransforms);

		renderingData.addPointLight(pointLightComponent.m_color, pointLightComponent.m_radius, nodeTransforms->m_globalTransform.getOrigin());
	}
}

void VT::PreparingRenderingDataSystem::prepareData(const ILevel& level, RenderingData& renderingData)
{
	renderingData.clear();

	const IScene* scene = level.getScene();
	const EntityComponentSystem* ecs = level.getEntityComponentSystem();

	renderingData.setCameraTransforms();

	prepareMeshData(scene, ecs, renderingData);
	prepareLightData(scene, ecs, renderingData);
}
