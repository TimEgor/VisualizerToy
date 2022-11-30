#include "ManagedResourceDependencyState.h"

#include "DummyResourceSystem.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

VT_DUMMY_RS::ManagedResourceDependencyState::ManagedResourceDependencyState(
	PackageRequestID requestID, const VT::ResourceDependencyState::Callback& callback)
	: VT::ResourceDependencyState(callback), m_requestID(requestID)
{
	assert(callback);
}

void VT_DUMMY_RS::ManagedResourceDependencyState::selfDestroy()
{
	reinterpret_cast<DummyResourceSystem*>(VT::EngineInstance::getInstance()->getEnvironment()->m_resourceSystem)->releaseDependencyState(this);
}
