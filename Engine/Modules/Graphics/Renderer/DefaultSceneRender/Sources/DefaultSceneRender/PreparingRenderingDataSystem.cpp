#include "PreparingRenderingDataSystem.h"

#include "DefaultRender/DefaultRenderingData.h"
#include "Scene/IScene.h"
#include "LevelSystem/ILevel.h"
#include "EntityComponentSystem/EntityComponentSystem.h"

#include "Scene/SceneNodeIDComponent.h"
#include "MeshSystem/MeshComponent.h"
#include "LightSystem/PointLightComponent.h"
#include "LightSystem/DirectionalLightComponent.h"
#include "Camera/PerspectiveCameraComponent.h"

#include "Math/ComputeMatrix.h"
#include "Math/ComputeVector.h"
#include "Math/Consts.h"

void VT::PreparingRenderingDataSystem::prepareMeshData(const IScene* scene, const EntityComponentSystem* ecs, DefaultRenderingData& renderingData)
{
	auto ecsView = ecs->getView<SceneNodeIDComponent, MeshComponent>();

	for (const auto& [entity, sceneNodeIDComponent, meshComponent] : ecsView.each())
	{
		if (!meshComponent.m_mesh)
		{
			continue;
		}

		NodeID nodeID = sceneNodeIDComponent.getNodeID();
		assert(!scene->isNodeDirty(nodeID));

		const Transform& nodeWorldTransform = scene->getNodeWorldTransform(nodeID);
		renderingData.addMesh(meshComponent.m_mesh, nodeWorldTransform.m_matrix);
	}
}

void VT::PreparingRenderingDataSystem::preparePointLightData(const IScene* scene, const EntityComponentSystem* ecs, DefaultRenderingData& renderingData)
{
	auto ecsView = ecs->getView<SceneNodeIDComponent, PointLightComponent>();

	for (const auto& [entity, sceneNodeIDComponent, pointLightComponent] : ecsView.each())
	{
		NodeID nodeID = sceneNodeIDComponent.getNodeID();
		assert(!scene->isNodeDirty(nodeID));

		const Transform& nodeWorldTransform = scene->getNodeWorldTransform(nodeID);
		renderingData.addPointLight(pointLightComponent.m_color, pointLightComponent.m_radius, pointLightComponent.m_intensity, nodeWorldTransform.getOrigin());
	}
}

void VT::PreparingRenderingDataSystem::prepareDirectionalLightData(const IScene* scene,
	const EntityComponentSystem* ecs, DefaultRenderingData& renderingData)
{
	auto ecsView = ecs->getView<SceneNodeIDComponent, DirectionalLightComponent>();

	for (const auto& [entity, sceneNodeIDComponent, dirLightComponent] : ecsView.each())
	{
		NodeID nodeID = sceneNodeIDComponent.getNodeID();
		assert(!scene->isNodeDirty(nodeID));

		const Transform& nodeWorldTransform = scene->getNodeWorldTransform(nodeID);
		renderingData.addDirectionalLight(dirLightComponent.m_color, dirLightComponent.m_intensity, nodeWorldTransform.getAxisZ());
	}
}

void VT::PreparingRenderingDataSystem::prepareCameraData(VT_Entity cameraEntity, const IScene* scene, const EntityComponentSystem* ecs,
                                                         DefaultRenderingData& renderingData)
{
	const PerspectiveCameraComponent& cameraComponent = ecs->getComponent<PerspectiveCameraComponent>(cameraEntity);
	const SceneNodeIDComponent& nodeIDComponent = ecs->getComponent<SceneNodeIDComponent>(cameraEntity);

	const Transform& cameraWorldTransform = scene->getNodeWorldTransform(nodeIDComponent.getNodeID());

	const Vector3& cameraPos = cameraWorldTransform.getOrigin();

	COMPUTE_MATH::ComputeMatrix viewTransform = COMPUTE_MATH::matrixLookToLH(
		COMPUTE_MATH::loadComputeVectorFromVector3(cameraPos),
		COMPUTE_MATH::loadComputeVectorFromVector3(cameraWorldTransform.getAxisZ()),
		COMPUTE_MATH::loadComputeVectorFromVector3(cameraWorldTransform.getAxisY())
	);

	CameraTransforms cameraTransforms;

	cameraTransforms.m_cameraPosition = cameraPos;

	cameraTransforms.nearPlane = cameraComponent.m_cameraData.m_nearClipPlane;
	cameraTransforms.farPlane = cameraComponent.m_cameraData.m_farClipPlane;

	COMPUTE_MATH::ComputeMatrix projectionTransform = COMPUTE_MATH::matrixPerspectiveFovLH(cameraComponent.m_cameraData.m_FoV * VT_DEG_TO_RAD,
		1.0f, cameraTransforms.nearPlane, cameraTransforms.farPlane);

	cameraTransforms.m_viewTransform = COMPUTE_MATH::saveComputeMatrixToMatrix4x4(viewTransform);
	cameraTransforms.m_projectionTransform = COMPUTE_MATH::saveComputeMatrixToMatrix4x4(projectionTransform);
	cameraTransforms.m_invProjectionTransform = COMPUTE_MATH::saveComputeMatrixToMatrix4x4(COMPUTE_MATH::matrixInverse(projectionTransform));

	renderingData.setCameraTransforms(cameraTransforms);
}

void VT::PreparingRenderingDataSystem::prepareData(const ILevel& level, DefaultRenderingData& renderingData)
{
	renderingData.clear();

	const IScene* scene = level.getScene();
	const EntityComponentSystem* ecs = level.getEntityComponentSystem();

	prepareMeshData(scene, ecs, renderingData);
	preparePointLightData(scene, ecs, renderingData);
	prepareDirectionalLightData(scene, ecs, renderingData);
	prepareCameraData(level.getCameraEntity(), scene, ecs, renderingData);
}
