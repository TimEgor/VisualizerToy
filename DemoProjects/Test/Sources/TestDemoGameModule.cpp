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

#include "Math/Matrix.h"
#include "ResourceSystem/IResourceSystem.h"

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
	VT::VT_Entity testEntity = level->createEntity();
	level->getEntityComponentSystem()->addComponent<VT::MeshComponent>(testEntity,
		getEngineEnvironment()->m_meshSystem->loadMesh("Cube.mesh"));
	VT::NodeID testNodeID = level->getEntityComponentSystem()->getComponent<VT::SceneNodeIDComponent>(testEntity).getNodeID();
	m_transforms = level->getScene()->getNodeTransforms(testNodeID);
}

void VT_DEMO_TEST::TestDemoGameModule::onUnloaded()
{
}

void VT_DEMO_TEST::TestDemoGameModule::update(float deltaTime)
{
	m_transforms->m_globalTransform.m_41 = sinf(m_time) * 3.0f;
	m_time += deltaTime;
}
