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

#include "Camera/PerspectiveCameraComponent.h"

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

	VT::VT_Entity light1Entity = level->createEntity();
	VT::PointLightComponent& light1 = ecs->addComponent<VT::PointLightComponent>(light1Entity);
	VT::NodeID light1NodeID = ecs->getComponent<VT::SceneNodeIDComponent>(light1Entity).getNodeID();

	VT::VT_Entity light2Entity = level->createEntity();
	VT::PointLightComponent& light2 = ecs->addComponent<VT::PointLightComponent>(light2Entity);
	VT::NodeID light2NodeID = ecs->getComponent<VT::SceneNodeIDComponent>(light2Entity).getNodeID();

	VT::VT_Entity light3Entity = level->createEntity();
	VT::PointLightComponent& light3 = ecs->addComponent<VT::PointLightComponent>(light3Entity);
	VT::NodeID light3NodeID = ecs->getComponent<VT::SceneNodeIDComponent>(light3Entity).getNodeID();

	light1.m_radius = 2.0f;
	light1.m_color = VT::Vector3(1.0f, 0.0f, 0.0f);
	scene->getNodeWorldTransformRaw(light1NodeID, false).getOrigin() = VT::Vector3(0.0f, 0.3f, -1.5f);
	scene->setDirtyState(light1NodeID, VT::IScene::DirtyStateVariants::WORLD_TRANSFORM);

	light2.m_radius = 2.0f;
	light2.m_color = VT::Vector3(0.0f, 1.0f, 0.0f);
	scene->getNodeWorldTransformRaw(light2NodeID, false).getOrigin() = VT::Vector3(0.3f, -0.3f, -1.5f);
	scene->setDirtyState(light2NodeID, VT::IScene::DirtyStateVariants::WORLD_TRANSFORM);

	light3.m_radius = 2.0f;
	light3.m_color = VT::Vector3(0.0f, 0.0f, 1.0f);
	scene->getNodeWorldTransformRaw(light3NodeID, false).getOrigin() = VT::Vector3(-0.3f, -0.3f, -1.5f);
	scene->setDirtyState(light3NodeID, VT::IScene::DirtyStateVariants::WORLD_TRANSFORM);
}

void VT_DEMO_TEST::TestDemoGameModule::onUnloaded()
{
}

void VT_DEMO_TEST::TestDemoGameModule::update(float deltaTime)
{
	//VT::ILevel* level = getEngineEnvironment()->m_levelSystem->getCurrentLevel();
	//VT::IScene* scene = level->getScene();

	//scene->getNodeWorldTransformRaw(m_cubeNodeID, false).m_matrix.m_41 = sinf(m_time) * 3.0f;
	//scene->setDirtyState(m_cubeNodeID, VT::IScene::DirtyStateVariants::WORLD_TRANSFORM);

	VT_DEMO_COMMON::FreePerspectiveCameraController::updateCamera(m_cameraEntity, deltaTime);

	m_time += deltaTime;
}
