#include "TestDemoGameModule.h"

#include "PluginSystem/ExternalPluginCore.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "EntityComponentSystem/EntityComponentSystem.h"
#include "FreeCameraController/FreePerspectiveCameraController.h"

#include "LevelSystem/ILevelSystem.h"
#include "Scene/IScene.h"
#include "Scene/SceneNodeIDComponent.h"

#include "MeshSystem/IMeshSystem.h"
#include "MeshSystem/MeshComponent.h"

#include "ResourceSystem/IResourceSystem.h"
#include "LightSystem/PointLightComponent.h"
#include "LightSystem/DirectionalLightComponent.h"

#include "Camera/PerspectiveCameraComponent.h"
#include "Math/ComputeMath.h"
#include "Math/ComputeMatrix.h"
#include "Math/ComputeVector.h"
#include "Math/Consts.h"

#include "Math/Matrix.h"

VT_PLUGIN(VT_DEMO_TEST::TestDemoGameModule)

namespace VT_DEMO_TEST
{
	static VT::EngineEnvironment* getEngineEnvironment()
	{
		return VT::EngineInstance::getInstance().getEngine()->getEnvironment();
	}
}

void VT_DEMO_TEST::TestDemoGameModule::onLoaded()
{
	VT::ILevel* level = getEngineEnvironment()->m_levelSystem->createLevel();

	getEngineEnvironment()->m_levelSystem->setCurrentLevel(level);

	level->init();
	VT::EntityComponentSystem* ecs = level->getEntityComponentSystem();
	VT::IScene* scene = level->getScene();

	m_cameraEntity = level->createEntity();
	ecs->addComponent<VT::PerspectiveCameraComponent>(m_cameraEntity);
	VT::NodeID cameraNodeID = ecs->getComponent<VT::SceneNodeIDComponent>(m_cameraEntity).getNodeID();
	scene->getNodeWorldTransformRaw(cameraNodeID, false).getOrigin() = VT::Vector3(0.0f, 0.0f, -6.0f);
	scene->setDirtyState(cameraNodeID, VT::IScene::DirtyStateVariants::WORLD_TRANSFORM);

	level->setCameraEntity(m_cameraEntity);

	VT::VT_Entity cubeEntity = level->createEntity();
	ecs->addComponent<VT::MeshComponent>(cubeEntity,
		getEngineEnvironment()->m_meshSystem->loadMesh("Cube.mesh"));
	m_cubeNodeID = ecs->getComponent<VT::SceneNodeIDComponent>(cubeEntity).getNodeID();

	VT::VT_Entity pointLight1Entity = level->createEntity();
	VT::PointLightComponent& pointLight1 = ecs->addComponent<VT::PointLightComponent>(pointLight1Entity);
	VT::NodeID pointLight1NodeID = ecs->getComponent<VT::SceneNodeIDComponent>(pointLight1Entity).getNodeID();

	VT::VT_Entity pointLight2Entity = level->createEntity();
	VT::PointLightComponent& pointLight2 = ecs->addComponent<VT::PointLightComponent>(pointLight2Entity);
	VT::NodeID pointLight2NodeID = ecs->getComponent<VT::SceneNodeIDComponent>(pointLight2Entity).getNodeID();

	VT::VT_Entity pointLight3Entity = level->createEntity();
	VT::PointLightComponent& pointLight3 = ecs->addComponent<VT::PointLightComponent>(pointLight3Entity);
	VT::NodeID pointLight3NodeID = ecs->getComponent<VT::SceneNodeIDComponent>(pointLight3Entity).getNodeID();

	pointLight1.m_radius = 2.0f;
	pointLight1.m_color = VT::Vector3(1.0f, 0.0f, 0.0f);
	scene->getNodeWorldTransformRaw(pointLight1NodeID, false).getOrigin() = VT::Vector3(0.0f, 0.3f, -1.5f);
	scene->setDirtyState(pointLight1NodeID, VT::IScene::DirtyStateVariants::WORLD_TRANSFORM);

	pointLight2.m_radius = 2.0f;
	pointLight2.m_color = VT::Vector3(0.0f, 1.0f, 0.0f);
	scene->getNodeWorldTransformRaw(pointLight2NodeID, false).getOrigin() = VT::Vector3(0.3f, -0.3f, -1.5f);
	scene->setDirtyState(pointLight2NodeID, VT::IScene::DirtyStateVariants::WORLD_TRANSFORM);

	pointLight3.m_radius = 2.0f;
	pointLight3.m_color = VT::Vector3(0.0f, 0.0f, 1.0f);
	scene->getNodeWorldTransformRaw(pointLight3NodeID, false).getOrigin() = VT::Vector3(-0.3f, -0.3f, -1.5f);
	scene->setDirtyState(pointLight3NodeID, VT::IScene::DirtyStateVariants::WORLD_TRANSFORM);

	VT::VT_Entity dirLight1Entity = level->createEntity();
	VT::DirectionalLightComponent& dirLight1 = ecs->addComponent<VT::DirectionalLightComponent>(dirLight1Entity);
	VT::NodeID dirLight1NodeID = ecs->getComponent<VT::SceneNodeIDComponent>(dirLight1Entity).getNodeID();

	dirLight1.m_intensity = 1.0f;
	VT::COMPUTE_MATH::ComputeMatrix dirTransform = VT::COMPUTE_MATH::matrixRotationRollPithYaw(45.0f * VT::VT_DEG_TO_RAD, 45.0f * VT::VT_DEG_TO_RAD, 0.0f);
	scene->getNodeWorldTransformRaw(dirLight1NodeID, false).m_matrix = VT::COMPUTE_MATH::saveComputeMatrixToMatrix4x4(dirTransform);
	scene->setDirtyState(dirLight1NodeID, VT::IScene::DirtyStateVariants::WORLD_TRANSFORM);
}

void VT_DEMO_TEST::TestDemoGameModule::onUnloaded()
{
}

void VT_DEMO_TEST::TestDemoGameModule::update(float deltaTime)
{
	VT::ILevel* level = getEngineEnvironment()->m_levelSystem->getCurrentLevel();
	VT::IScene* scene = level->getScene();

	scene->getNodeWorldTransformRaw(m_cubeNodeID, false).m_matrix.m_41 = sinf(m_time) * 3.0f;
	scene->setDirtyState(m_cubeNodeID, VT::IScene::DirtyStateVariants::WORLD_TRANSFORM);

	VT_DEMO_COMMON::FreePerspectiveCameraController::updateCamera(m_cameraEntity, deltaTime);

	m_time += deltaTime;
}
