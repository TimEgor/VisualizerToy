#pragma once

#include "ResourceSystem/IResourceSystem.h"
#include "Multithreading/Mutex.h"

#include "ObjectPool/ThreadSafeIndexObjectPool.h"
#include "Containers/StaticVector.h"

#include <unordered_map>

#define VT_RESOURCE_SYSTEM_DUMMY_TYPE

namespace VT_DUMMY_RS
{
	using ManagedResourceDataID = uint32_t;
	constexpr ManagedResourceDataID InvalidManagedResourceDataID = 0;

	class ManagedResourceData final : public VT::ResourceData
	{
	private:
		ManagedResourceDataID m_id = InvalidManagedResourceDataID;

		virtual void selfDestroy() override;

	public:
		ManagedResourceData(ManagedResourceDataID id)
			: m_id(id) {}
		 
		void* getData() { return m_data; }
		void setData(void* data, size_t dataSize)
		{
			m_data = data;
			m_dataSize = dataSize;
		}

		ManagedResourceDataID getID() const { return m_id; }
		void setState(VT::ResourceState state) { VT::storeAtomic(&m_state, state, VT::MemoryOrder::Relaxed); }
	};

	class ResourceLoader;

	class DummyResourceSystem final : public VT::IResourceSystem
	{
		friend ManagedResourceData;
		friend ResourceLoader;

		using ResourceDataContainerType = std::unordered_map<ManagedResourceDataID, ManagedResourceData>;

		using EventID = uint32_t;
		using ResourceEventContainer = VT::StaticVector<EventID, 3>;
		using ResourceEventCollection = std::unordered_map<VT::FileNameID, ResourceEventContainer>;
		using EventCollection = VT::ThreadSafeIndexObjectPool<VT::IResourceSystem::LoadedCallback, EventID>;

	private:
		ResourceDataContainerType m_datas;
		ResourceEventCollection m_resourceEvents;
		EventCollection m_events;

		VT::Mutex m_mutex;

		ResourceLoader* m_loader = nullptr;

		ManagedResourceDataID getResourceID(const VT::FileName& name) const;

		void onResourceLoaded(VT::FileNameID resID, VT::ResourceDataReference resource);
		void releaseResourceData(ManagedResourceData* data);

	public:
		DummyResourceSystem() = default;
		~DummyResourceSystem() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual VT::ResourceDataReference getResource(const VT::FileName& resName) override;
		virtual VT::ResourceDataReference getResourceAsync(const VT::FileName& resName, const std::function<void(VT::ResourceDataReference)>& callback) override;

		VT_RESOURCE_SYSTEM_TYPE(VT_RESOURCE_SYSTEM_DUMMY_TYPE)
	};
}