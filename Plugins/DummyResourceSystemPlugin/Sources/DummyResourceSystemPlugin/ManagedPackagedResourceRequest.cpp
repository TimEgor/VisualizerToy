#include "ManagedPackagedResourceRequest.h"

#include "DummyResourceSystem.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

VT_DUMMY_RS::ManagedPackagedResourceRequest::ManagedPackagedResourceRequest(
	PackageRequestID requestID, size_t requestedResourceCount, const VT::PackageRequestCallback& callback)
	: m_requestedResourceCount(requestedResourceCount), m_callback(callback), m_requestID(requestID)
{
	assert(callback);
	m_result.resize(requestedResourceCount);
}

void VT_DUMMY_RS::ManagedPackagedResourceRequest::selfDestroy()
{
	reinterpret_cast<DummyResourceSystem*>(VT::EngineInstance::getInstance()->getEnvironment()->m_resourceSystem)->releasePackageRequest(this);
}

void VT_DUMMY_RS::ManagedPackagedResourceRequest::setResourceResult(size_t index, VT::ResourceDataReference resource)
{
	m_result[index] = resource;

	size_t prevVal = VT::fetchAddAtomic(&m_loadedResourceCount, 1, VT::MemoryOrder::Acq_Rel);
	if (prevVal + 1 == m_requestedResourceCount)
	{
		m_callback(this);
	}
}
