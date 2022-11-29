#pragma once

#include "ManagedResourceData.h"
#include "ManagedPackagedResourceRequest.h"

#include "Multithreading/Mutex.h"

#include "ObjectPool/ThreadSafeIndexObjectPool.h"
#include "Containers/StaticVector.h"

#include <unordered_map>

#define VT_RESOURCE_SYSTEM_DUMMY_TYPE

namespace VT_DUMMY_RS
{
	class ResourceLoader;

	using EventID = uint32_t;
	constexpr EventID InvalidEventID = -1;

	class DummyResourceSystem final : public VT::IResourceSystem
	{
		friend ManagedResourceData;
		friend ManagedPackagedResourceRequest;
		friend ResourceLoader;

		using ResourceDataContainerType = std::unordered_map<ManagedResourceDataID, ManagedResourceData>;

		using ResourceEventContainer = VT::StaticVector<EventID, 3>;
		using ResourceEventCollection = std::unordered_map<ManagedResourceDataID, ResourceEventContainer>;
		using EventCollection = VT::ThreadSafeIndexObjectPool<VT::IResourceSystem::LoadedResourceCallback, EventID>;

		using PackageRequestCollection = VT::ThreadSafeIndexObjectPool<ManagedPackagedResourceRequest, PackageRequestID>;

		using LoadedResourceCallback = VT::IResourceSystem::LoadedResourceCallback;

	private:
		ResourceDataContainerType m_datas;

		ResourceEventCollection m_resourceEvents;
		EventCollection m_events;

		PackageRequestCollection m_packageRequests;

		VT::Mutex m_resourceRequestMutex;
		VT::Mutex m_resoureEventMutex;

		ResourceLoader* m_loader = nullptr;

		ManagedResourceDataID getResourceID(const VT::FileName& name) const;

		void onResourceLoaded(ManagedResourceDataID resID, VT::ResourceDataReference resource);
		void releaseResourceData(ManagedResourceData* data);
		void releasePackageRequest(ManagedPackagedResourceRequest* request);

		EventID addEvent(const LoadedResourceCallback& callback);
		void addResourceEvent(ManagedResourceDataID resourceID, const LoadedResourceCallback& callback);

		ManagedResourceDataID getResourceAsyncInternal(const VT::FileName& resName, const LoadedResourceCallback& callback);

	public:
		DummyResourceSystem() = default;
		~DummyResourceSystem() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual VT::ResourceDataReference getResource(const VT::FileName& resName) override;
		virtual void getResourceAsync(const VT::FileName& resName, const LoadedResourceCallback& callback) override;

		virtual size_t getPackagedResource(const PackageResourceRequestCollection& request, PackageResourceRequestResultCollection& result) override;
		virtual void getPackagedResourceAsync(const DelayedPackageResourceRequestCollection& request, const VT::PackageRequestCallback& callback = nullptr) override;

		VT_RESOURCE_SYSTEM_TYPE(VT_RESOURCE_SYSTEM_DUMMY_TYPE)
	};
}