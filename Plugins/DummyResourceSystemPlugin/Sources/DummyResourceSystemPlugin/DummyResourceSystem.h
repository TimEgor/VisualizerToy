#pragma once

#include "ManagedResourceData.h"
#include "ManagedResourceDependencyState.h"

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
		friend ManagedResourceDependencyState;
		friend ResourceLoader;

		using ResourceDataContainerType = std::unordered_map<ManagedResourceDataID, ManagedResourceData>;

		using ResourceEventContainer = VT::StaticVector<EventID, 3>;
		using ResourceEventCollection = std::unordered_map<ManagedResourceDataID, ResourceEventContainer>;
		using EventCollection = VT::ThreadSafeIndexObjectPool<VT::IResourceSystem::LoadingResourceCallback, EventID>;

		using DependencyStateCollection = VT::ThreadSafeIndexObjectPool<ManagedResourceDependencyState, PackageRequestID>;

		using LoadingResourceCallback = VT::IResourceSystem::LoadingResourceCallback;

	private:
		ResourceDataContainerType m_datas;

		ResourceEventCollection m_resourceEvents;
		EventCollection m_events;

		DependencyStateCollection m_dependencyStates;

		VT::Mutex m_resourceRequestMutex;
		VT::Mutex m_resoureEventMutex;

		ResourceLoader* m_loader = nullptr;

		ManagedResourceDataID getResourceID(const VT::FileName& name) const;

		void onResourceLoaded(ManagedResourceDataID resID, VT::ResourceDataReference resource);
		void releaseResourceData(ManagedResourceData* data);
		void releaseDependencyState(ManagedResourceDependencyState* state);

		EventID addEvent(const LoadingResourceCallback& callback);
		void addResourceEvent(ManagedResourceDataID resourceID, const LoadingResourceCallback& callback);

	public:
		DummyResourceSystem() = default;
		~DummyResourceSystem() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual VT::ResourceDataReference getResource(const VT::FileName& resName) override;
		virtual void getResourceAsync(const VT::FileName& resName, const LoadingResourceCallback& callback) override;

		virtual VT::ResourceDependencyStateReference createResourceDependencyState(const VT::ResourceDependencyState::Callback& callback) override;

		VT_RESOURCE_SYSTEM_TYPE(VT_RESOURCE_SYSTEM_DUMMY_TYPE)
	};
}