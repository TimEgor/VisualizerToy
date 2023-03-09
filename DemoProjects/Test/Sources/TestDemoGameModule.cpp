#include "TestDemoGameModule.h"

#include "PluginSystem/ExternalPluginCore.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "EntityComponentSystem/EntityComponentSystem.h"

#include "LevelSystem/ILevelSystem.h"
#include "Scene/IScene.h"
#include "Scene/SceneNodeIDComponent.h"

#include "MeshSystem/IMeshSystem.h"
#include "MeshSystem/MeshComponent.h"

#include "ResourceSystem/IResourceSystem.h"
#include "LightSystem/PointLightComponent.h"

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
	VT::VT_Entity cubeEntity = level->createEntity();
	level->getEntityComponentSystem()->addComponent<VT::MeshComponent>(cubeEntity,
		getEngineEnvironment()->m_meshSystem->loadMesh("Cube.mesh"));
	VT::NodeID testNodeID = level->getEntityComponentSystem()->getComponent<VT::SceneNodeIDComponent>(cubeEntity).getNodeID();
	m_transforms = level->getScene()->getNodeTransforms(testNodeID);
	m_transforms->m_globalTransform.getOrigin().m_x = 3.0f;

	VT::VT_Entity light1Entity = level->createEntity();
	VT::PointLightComponent& light1 = level->getEntityComponentSystem()->addComponent<VT::PointLightComponent>(light1Entity);
	VT::NodeID light1NodeID = level->getEntityComponentSystem()->getComponent<VT::SceneNodeIDComponent>(light1Entity).getNodeID();

	VT::VT_Entity light2Entity = level->createEntity();
	VT::PointLightComponent& light2 = level->getEntityComponentSystem()->addComponent<VT::PointLightComponent>(light2Entity);
	VT::NodeID light2NodeID = level->getEntityComponentSystem()->getComponent<VT::SceneNodeIDComponent>(light2Entity).getNodeID();

	VT::VT_Entity light3Entity = level->createEntity();
	VT::PointLightComponent& light3 = level->getEntityComponentSystem()->addComponent<VT::PointLightComponent>(light3Entity);
	VT::NodeID light3NodeID = level->getEntityComponentSystem()->getComponent<VT::SceneNodeIDComponent>(light3Entity).getNodeID();

	light1.m_radius = 2.0f;
	light1.m_color = VT::Vector3(1.0f, 0.0f, 0.0f);
	level->getScene()->getNodeTransforms(light1NodeID)->m_globalTransform.getOrigin() = VT::Vector3(0.0f, 0.3f, -1.5f);

	light2.m_radius = 2.0f;
	light2.m_color = VT::Vector3(0.0f, 1.0f, 0.0f);
	level->getScene()->getNodeTransforms(light2NodeID)->m_globalTransform.getOrigin() = VT::Vector3(0.3f, -0.3f, -1.5f);

	light3.m_radius = 2.0f;
	light3.m_color = VT::Vector3(0.0f, 0.0f, 1.0f);
	level->getScene()->getNodeTransforms(light3NodeID)->m_globalTransform.getOrigin() = VT::Vector3(-0.3f, -0.3f, -1.5f);
}

void VT_DEMO_TEST::TestDemoGameModule::onUnloaded()
{
}

void VT_DEMO_TEST::TestDemoGameModule::update(float deltaTime)
{
	m_transforms->m_globalTransform.m_matrix.m_41 = sinf(m_time) * 3.0f;
	m_time += deltaTime;
}
