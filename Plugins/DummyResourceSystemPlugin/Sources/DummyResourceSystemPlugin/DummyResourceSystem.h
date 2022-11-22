#pragma once

#include "ResourceSystem/IResourceSystem.h"
#include "Multithreading/Mutex.h"

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

		using ResourceDataContainerType = std::unordered_map<ManagedResourceDataID, ManagedResourceData>;

	private:
		ResourceDataContainerType m_datas;
		VT::Mutex m_mutex;

		ResourceLoader* m_loader = nullptr;

		ManagedResourceDataID getResourceID(const VT::FileName& name) const;

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