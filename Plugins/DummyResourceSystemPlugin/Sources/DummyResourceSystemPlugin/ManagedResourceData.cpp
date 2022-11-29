#include "ManagedResourceData.h"

#include "DummyResourceSystem.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

namespace VT_DUMMY_RS
{
	void ManagedResourceData::selfDestroy()
	{
		if (VT::loadAtomic(&m_state, VT::MemoryOrder::Acquire) == VT::ResourceState::LOADED)
		{
			reinterpret_cast<DummyResourceSystem*>(VT::EngineInstance::getInstance()->getEnvironment()->m_resourceSystem)->releaseResourceData(this);
		}
	}

	void ManagedResourceData::setData(void* data, size_t dataSize)
	{
		m_data = data;
		m_dataSize = dataSize;
	}

	void ManagedResourceData::setState(VT::ResourceState state)
	{
		VT::storeAtomic(&m_state, state, VT::MemoryOrder::Release);
	}
}