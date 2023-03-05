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

	VT::VT_Entity lightEntity = level->createEntity();
	VT::PointLightComponent& light = level->getEntityComponentSystem()->addComponent<VT::PointLightComponent>(lightEntity);
	VT::NodeID lightNodeID = level->getEntityComponentSystem()->getComponent<VT::SceneNodeIDComponent>(lightEntity).getNodeID();

	light.m_radius = 3.0f;
	light.m_color = VT::Vector3Identity;
	level->getScene()->getNodeTransforms(lightNodeID)->m_globalTransform.getOrigin().m_z = -1.5f;
}

void VT_DEMO_TEST::TestDemoGameModule::onUnloaded()
{
}

void VT_DEMO_TEST::TestDemoGameModule::update(float deltaTime)
{
	m_transforms->m_globalTransform.m_matrix.m_41 = sinf(m_time) * 3.0f;
	m_time += deltaTime;
}
