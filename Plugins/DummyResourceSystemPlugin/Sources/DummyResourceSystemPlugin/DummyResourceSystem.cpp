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
		VT_CHECK_INITIALIZATION(m_loader && m_loader->init(this));

		VT_CHECK_INITIALIZATION(m_events.init());
		m_resourceEvents.reserve(4096);

		return true;
	}

	void DummyResourceSystem::release()
	{
		m_resourceEvents = ResourceEventCollection();
		m_events.release();

		VT_SAFE_DESTROY_WITH_RELEASING(m_loader);
	}

	ManagedResourceDataID DummyResourceSystem::getResourceID(const VT::FileName& resName) const
	{
		return resName.hash();
	}

	void DummyResourceSystem::onResourceLoaded(VT::FileNameID resID, VT::ResourceDataReference resource)
	{
		auto fintResourceEventsIter = m_resourceEvents.find(resID);
		if (fintResourceEventsIter != m_resourceEvents.end())
		{
			for (EventID eventID : fintResourceEventsIter->second)
			{
				VT::IResourceSystem::LoadedCallback& callback = *m_events.getElement(eventID);
				if (callback)
				{
					callback(resource);
				}

				m_events.removeElement(eventID);
			}
		}

		VT::LockGuard<VT::Mutex> locker(m_mutex);
		m_resourceEvents.erase(resID);
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
		assert(resName);
		assert(m_loader);

		ManagedResourceDataID resourceID = getResourceID(resName);

		VT::UniqueLockGuard<VT::Mutex> locker(m_mutex);

		auto findDataIter = m_datas.find(resourceID);
		if (findDataIter != m_datas.end())
		{
			VT::ResourceDataReference resourceRef = &findDataIter->second;

			if (callback)
			{
				if (resourceRef->getState() == VT::ResourceState::LOADING)
				{
					EventCollection::NewElementInfo newEventInfo = m_events.addElement(callback);
					m_resourceEvents[resourceID].emplaceBack(newEventInfo.m_elementIndex);
				}
				else if (resourceRef->getState() == VT::ResourceState::LOADED)
				{
					locker.unlock();
					callback(resourceRef);
				}
			}

			return resourceRef;
		}

		auto newVal = m_datas.insert(std::make_pair(resourceID, std::move(ManagedResourceData(resourceID))));

		EventCollection::NewElementInfo newEventInfo = m_events.addElement(callback);
		m_resourceEvents[resourceID].emplaceBack(newEventInfo.m_elementIndex);

		locker.unlock();

		ManagedResourceData& newResource = newVal.first->second;

		m_loader->loadResourceAsync(resName, newResource);

		return &newResource;
	}
}