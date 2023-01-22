#include "PreparingRenderingDataSystem.h"

#include "RenderingData.h"
#include "Scene/IScene.h"
#include "LevelSystem/ILevel.h"
#include "EntityComponentSystem/EntityComponentSystem.h"
#include "MeshSystem/MeshComponent.h"
#include "Scene/SceneNodeIDComponent.h"

void VT::PreparingRenderingDataSystem::prepareData(const ILevel& level, RenderingData& renderingData)
{
	renderingData.clear();

	const IScene* scene = level.getScene();
	const EntityComponentSystem* ecs = level.getEntityComponentSystem();
	auto ecsView = ecs->getView<SceneNodeIDComponent, MeshComponent>();

	for (const auto& [entity, sceneNodeIDComponent, meshComponent] : ecsView.each())
	{
		if (!meshComponent.m_mesh)
		{
			continue;
		}

		const NodeTransforms* nodeTransforms = scene->getNodeTransforms(sceneNodeIDComponent.getNodeID());
		assert(nodeTransforms);

		renderingData.addMesh(meshComponent.m_mesh, nodeTransforms->m_globalTransform);
	}
}
