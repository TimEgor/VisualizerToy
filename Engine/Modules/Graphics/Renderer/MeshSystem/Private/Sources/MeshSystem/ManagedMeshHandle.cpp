#include "ManagedMeshHandle.h"

#include "MeshSystem.h"
#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

namespace VT
{
	class MeshSystem;

	MeshSystem* getMeshSystem()
	{
		return reinterpret_cast<MeshSystem*>(EngineInstance::getInstance()->getEnvironment()->m_meshSystem);
	}
}

void VT::ManagedMeshHandle::selfDestroy()
{
	getMeshSystem()->deleteMesh(m_mesh);
	getMeshSystem()->deleteMeshHandle(m_id);
}

void VT::NamedManagedMeshHandle::selfDestroy()
{
	getMeshSystem()->deleteMesh(m_mesh);
	getMeshSystem()->deleteNamedMeshHandle(m_nameID);
}
