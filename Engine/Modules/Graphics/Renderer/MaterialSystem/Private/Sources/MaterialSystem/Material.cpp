#include "Material.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "MaterialSystem.h"

inline VT::MaterialSystem* getMaterialSystem()
{
	return reinterpret_cast<VT::MaterialSystem*>(VT::EngineInstance::getInstance()->getEnvironment()->m_materialSystem);
}

VT::ManagedMaterial::~ManagedMaterial()
{
	getMaterialSystem()->deleteMaterial(m_id);
}
