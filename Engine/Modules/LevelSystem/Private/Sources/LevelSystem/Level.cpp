#include "Level.h"

#include "Core/UtilitiesMacros.h"
#include "Scene/Scene.h"
#include "Scene/SceneNodeIDComponent.h"

bool VT::Level::init()
{
	m_entityComponentSystem = new EntityComponentSystem();
	VT_CHECK_INITIALIZATION(m_entityComponentSystem && m_entityComponentSystem->init());

	m_scene = new Scene();
	VT_CHECK_INITIALIZATION(m_scene && m_scene->init());

	m_nodeEnities.reserve(1024);

	return true;
}

void VT::Level::release()
{
	VT_SAFE_DESTROY_WITH_RELEASING(m_entityComponentSystem);
	VT_SAFE_DESTROY_WITH_RELEASING(m_scene);

	m_nodeEnities = NodeEntityCollection();
}

VT::VT_Entity VT::Level::createEntity()
{
	VT_Entity entity = m_entityComponentSystem->createEntity();
	NodeID nodeID = m_scene->addNode();

	m_entityComponentSystem->addComponent<SceneNodeIDComponent>(entity, nodeID);

	return entity;
}

VT::VT_Entity VT::Level::createEntity(VT_Entity parentEntity)
{
	SceneNodeIDComponent& parentNodeIDComponent = m_entityComponentSystem->getComponent<SceneNodeIDComponent>(parentEntity);

	VT_Entity entity = m_entityComponentSystem->createEntity();
	NodeID nodeID = m_scene->addNode(parentNodeIDComponent.getNodeID());

	m_entityComponentSystem->addComponent<SceneNodeIDComponent>(entity, nodeID);

	return entity;
}

void VT::Level::removeEntity(VT_Entity entity)
{
	SceneNodeIDComponent& nodeIDComponent = m_entityComponentSystem->getComponent<SceneNodeIDComponent>(entity);
	m_scene->removeNode(nodeIDComponent.getNodeID(),
		[this](NodeID nodeID) {
			VT_Entity entity = m_nodeEnities.at(nodeID);
			m_entityComponentSystem->destroyEntity(entity);
		}
	);
}

void VT::Level::setCameraEntity(VT_Entity entity)
{
	m_cameraEntity = entity;
}

VT::VT_Entity VT::Level::getCameraEntity() const
{
	return m_cameraEntity;
}
