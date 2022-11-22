#include "DummyResourceSystem.h"

#include "ResourceLoader.h"

#include "Multithreading/LockGuard.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

namespace VT_DUMMY_RS
{
	void ManagedResourceData::selfDestroy()
	{
		if (VT::loadAtomic(&m_state, VT::MemoryOrder::Relaxed) == VT::ResourceState::LOADED)
		{
			reinterpret_cast<DummyResourceSystem*>(VT::EngineInstance::getInstance()->getEnvironment()->m_resourceSystem)->releaseResourceData(this);
		}
	}

	bool DummyResourceSystem::init()
	{
		m_loader = new ResourceLoader();
		VT_CHECK_INITIALIZATION(m_loader && m_loader->init());

		return true;
	}

	void DummyResourceSystem::release()
	{
		VT_SAFE_DESTROY_WITH_RELEASING(m_loader);
	}

	ManagedResourceDataID DummyResourceSystem::getResourceID(const VT::FileName& resName) const
	{
		return resName.hash();
	}

	void DummyResourceSystem::releaseResourceData(ManagedResourceData* resData)
	{
		assert(resData);

		void* data = resData->getData();
		VT_SAFE_DESTROY_ARRAY(data);

		VT::LockGuard<VT::Mutex> locker(m_mutex);

		auto findDataIter = m_datas.find(resData->getID());
		if (findDataIter != m_datas.end())
		{
			m_datas.erase(findDataIter);
		}
	}

	VT::ResourceDataReference DummyResourceSystem::getResource(const VT::FileName& resName)
	{
		assert(resName);
		assert(m_loader);

		ManagedResourceDataID resourceID = getResourceID(resName);

		VT::UniqueLockGuard<VT::Mutex> locker(m_mutex);

		auto findDataIter = m_datas.find(resourceID);
		if (findDataIter != m_datas.end())
		{
			return &findDataIter->second;
		}

		auto newVal = m_datas.insert(std::make_pair(resourceID, std::move(ManagedResourceData(resourceID))));

		locker.unlock();

		ManagedResourceData& newResource = newVal.first->second;

		m_loader->loadResource(resName, newResource);

		return &newResource;
	}

	VT::ResourceDataReference DummyResourceSystem::getResourceAsync(const VT::FileName& resName, const std::function<void(VT::ResourceDataReference)>& callback)
	{
		return VT::ResourceDataReference();
	}
}