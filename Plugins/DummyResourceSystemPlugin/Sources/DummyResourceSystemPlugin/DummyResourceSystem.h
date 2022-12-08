#pragma once

#include "ManagedResourceData.h"
#include "ManagedResourceDependencyState.h"
#include "ManagedResourceSystemConverterArgs.h"

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
		friend ManagedResourceSystemConverterArgs;
		friend ResourceLoader;

		using LoadingResourceCallback = VT::IResourceSystem::LoadingResourceCallback;

		using ResourceDataContainerType = std::unordered_map<ManagedResourceDataID, ManagedResourceData>;

		using ResourceEventContainer = VT::StaticVector<EventID, 3>;
		using ResourceEventCollection = std::unordered_map<ManagedResourceDataID, ResourceEventContainer>;
		using EventCollection = VT::ThreadSafeIndexObjectPool<LoadingResourceCallback, EventID>;

		using DependencyStateCollection = VT::ThreadSafeIndexObjectPool<ManagedResourceDependencyState, PackageRequestID>;

	private:
		ResourceDataContainerType m_datas;

		ResourceEventCollection m_resourceEvents;
		EventCollection m_events;

		DependencyStateCollection m_dependencyStates;

		ConverterArgsColllection m_converterArgsCollection;

		VT::Mutex m_resourceRequestMutex;
		VT::Mutex m_resoureEventMutex;

		ResourceLoader* m_loader = nullptr;

		ManagedResourceDataID getResourceID(const VT::FileName& name) const;

		void onResourceLoaded(ManagedResourceDataID resID, VT::ResourceDataReference resource);
		void releaseResourceData(ManagedResourceData* data);
		void releaseDependencyState(ManagedResourceDependencyState* state);

		EventID addEvent(const LoadingResourceCallback& callback);
		void addResourceEvent(ManagedResourceDataID resourceID, const LoadingResourceCallback& callback);

		virtual VT::ResourceSystemConverterArgs* allocateResourceConverterArgs(VT::ResourceConverterType type) override;
		virtual void addResourceConverterArgs(VT::ResourceConverterType type, VT::IResourceSystemConverterArgsDestructor* destructor, size_t argsSize) override;
		void releaseResourceSystemConverterArgs(ManagedResourceSystemConverterArgs* args);

	public:
		DummyResourceSystem() = default;
		~DummyResourceSystem() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual void addResourceConverter(VT::IFileResourceConverter* converter) override;
		virtual void removeResourceConverter(VT::ResourceConverterType converterType) override;

		virtual VT::ResourceDataReference getResource(const VT::FileName& resName, VT::ResourceSystemConverterArgsReference args) override;
		virtual void getResourceAsync(const VT::FileName& resName, VT::ResourceSystemConverterArgsReference args, const LoadingResourceCallback& callback) override;

		virtual VT::ResourceDependencyStateReference createResourceDependencyState(const VT::ResourceDependencyState::Callback& callback) override;

		VT_RESOURCE_SYSTEM_TYPE(VT_RESOURCE_SYSTEM_DUMMY_TYPE)
	};
}