#include "EntityComponentSystem.h"

bool VT::EntityComponentSystem::init()
{
	return true;
}

void VT::EntityComponentSystem::release()
{
}

VT::VT_Entity VT::EntityComponentSystem::createEntity()
{
	return m_registry.create();
}

void VT::EntityComponentSystem::destroyEntity(VT_Entity entity)
{
	m_registry.destroy(entity);
}
