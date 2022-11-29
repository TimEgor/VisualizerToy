#include "DummyResourceSystem.h"

#include "ResourceLoader.h"

#include "Multithreading/LockGuard.h"

namespace VT_DUMMY_RS
{
	bool DummyResourceSystem::init()
	{
		m_loader = new ResourceLoader();
		VT_CHECK_INITIALIZATION(m_loader && m_loader->init(this));

		VT_CHECK_INITIALIZATION(m_events.init());
		m_resourceEvents.reserve(4096);

		VT_CHECK_INITIALIZATION(m_packageRequests.init());

		return true;
	}

	void DummyResourceSystem::release()
	{
		m_packageRequests.release();

		m_resourceEvents = ResourceEventCollection();
		m_events.release();

		VT_SAFE_DESTROY_WITH_RELEASING(m_loader);
	}

	ManagedResourceDataID DummyResourceSystem::getResourceID(const VT::FileName& resName) const
	{
		return resName.hash();
	}

	void DummyResourceSystem::onResourceLoaded(ManagedResourceDataID resID, VT::ResourceDataReference resource)
	{
		auto fintResourceEventsIter = m_resourceEvents.find(resID);
		if (fintResourceEventsIter != m_resourceEvents.end())
		{
			for (EventID eventID : fintResourceEventsIter->second)
			{
				VT::IResourceSystem::LoadedResourceCallback& callback = *m_events.getElement(eventID);
				if (callback)
				{
					callback(resource);
				}

				m_events.removeElement(eventID);
			}
		}

		VT::LockGuard<VT::Mutex> locker(m_resoureEventMutex);
		m_resourceEvents.erase(resID);
	}

	void DummyResourceSystem::releaseResourceData(ManagedResourceData* resData)
	{
		assert(resData);

		void* data = resData->getData();
		VT_SAFE_DESTROY_ARRAY(data);

		VT::LockGuard<VT::Mutex> locker(m_resourceRequestMutex);

		auto findDataIter = m_datas.find(resData->getID());
		if (findDataIter != m_datas.end())
		{
			m_datas.erase(findDataIter);
		}
	}

	void DummyResourceSystem::releasePackageRequest(ManagedPackagedResourceRequest* request)
	{
		assert(request);
		m_packageRequests.removeElement(request->getRequestID());
	}

	EventID DummyResourceSystem::addEvent(const LoadedResourceCallback& callback)
	{
		return m_events.addElement(callback).m_elementIndex;
	}

	void DummyResourceSystem::addResourceEvent(ManagedResourceDataID resourceID, const LoadedResourceCallback& callback)
	{
		if (callback)
		{
			VT::LockGuard<VT::Mutex> locker(m_resoureEventMutex);
			m_resourceEvents[resourceID].emplaceBack(addEvent(callback));
		}
	}

	ManagedResourceDataID DummyResourceSystem::getResourceAsyncInternal(const VT::FileName& resName, const LoadedResourceCallback& callback)
	{
		assert(resName);
		assert(m_loader);

		ManagedResourceDataID resourceID = getResourceID(resName);

		VT::UniqueLockGuard<VT::Mutex> locker(m_resourceRequestMutex);

		auto findDataIter = m_datas.find(resourceID);
		if (findDataIter != m_datas.end())
		{
			VT::ResourceDataReference resourceRef = &findDataIter->second;
			locker.unlock();

			if (callback)
			{
				if (resourceRef->getState() == VT::ResourceState::LOADING)
				{
					addResourceEvent(resourceID, callback);
				}
				else
				{
					callback(resourceRef);
				}
			}

			return resourceID;
		}

		auto newVal = m_datas.insert(std::make_pair(resourceID, std::move(ManagedResourceData(resourceID))));
		locker.unlock();

		addResourceEvent(resourceID, callback);

		ManagedResourceData& newResource = newVal.first->second;

		m_loader->loadResourceAsync(resName, &newResource);

		return resourceID;
	}

	VT::ResourceDataReference DummyResourceSystem::getResource(const VT::FileName& resName)
	{
		assert(resName);
		assert(m_loader);

		ManagedResourceDataID resourceID = getResourceID(resName);

		VT::UniqueLockGuard<VT::Mutex> locker(m_resourceRequestMutex);

		auto findDataIter = m_datas.find(resourceID);
		if (findDataIter != m_datas.end())
		{
			return &findDataIter->second;
		}

		auto newVal = m_datas.insert(std::make_pair(resourceID, std::move(ManagedResourceData(resourceID))));

		locker.unlock();

		ManagedResourceData& newResource = newVal.first->second;

		m_loader->loadResource(resName, &newResource);

		return &newResource;
	}

	void DummyResourceSystem::getResourceAsync(const VT::FileName& resName, const LoadedResourceCallback& callback)
	{
		getResourceAsyncInternal(resName, callback);
	}

	size_t DummyResourceSystem::getPackagedResource(const PackageResourceRequestCollection& request, PackageResourceRequestResultCollection& result)
	{
		result.clear();
		result.resize(request.size());

		size_t loadedResurses = 0;

		for (size_t i = 0; i < request.size(); ++i)
		{
			VT::ResourceDataReference resource = getResource(request[i]);

			assert(resource);
			if (resource->getState() == VT::ResourceState::LOADED)
			{
				++loadedResurses;
			}

			result[i] = resource;
		}

		return loadedResurses;
	}

	void DummyResourceSystem::getPackagedResourceAsync(const DelayedPackageResourceRequestCollection& request, const VT::PackageRequestCallback& callback)
	{
		const size_t requestCount = request.size();

		PackageRequestCollection::NewElementInfo requestInfo = m_packageRequests.addElementRaw();
		ManagedPackagedResourceRequest* packageRequest
			= new (requestInfo.m_elementPtr) ManagedPackagedResourceRequest(requestInfo.m_elementIndex, requestCount, callback);

		for (size_t requestIndex = 0; requestIndex < requestCount; ++requestIndex)
		{
			const VT::IResourceSystem::DelayedResourceRequest& resourceRequest = request[requestIndex];

			auto requestCallback = [
				packageReq = packageRequest,
				index = requestIndex,
				resourceCallback = resourceRequest.m_callback
			](VT::ResourceDataReference resource)
				{
					if (resourceCallback)
					{
						resourceCallback(resource);
					}

					packageReq->setResourceResult(index, resource);
				};

			ManagedResourceDataID resourceID = getResourceAsyncInternal(resourceRequest.m_resourceName, requestCallback);
		}
	}
}