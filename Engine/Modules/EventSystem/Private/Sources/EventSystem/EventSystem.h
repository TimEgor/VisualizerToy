#pragma once

#include "EventSystem/IEventSystem.h"
#include "ObjectPool/ObjectPool.h"

namespace VT
{
	class EventSystem final : public IEventSystem
	{
		struct DispatcherData final
		{
			EventDispatcher m_dispatcher;
			EventType m_eventType = InvalidEventType;
			uint32_t m_orderIndex = -1;
		};

		using DispatcherPool = ObjectPool<DispatcherData, ObjectPoolHandle32>;
		using DispatcherPoolElementInfo = DispatcherPool::NewElementInfo;
		using DispatcherOrderCollection = std::vector<DispatcherData*>;
		using DispatcherOrdersCollection = std::unordered_map<EventType, DispatcherOrderCollection>;

		struct InstancedDispatcherData final
		{
			InstancedEventDispatcher m_dispatcher;
			InstancedEventID m_eventID = InvalidInstancedEventID;
			InstancedEventDispatcherID m_prevSiblingID = InvalidInstancedEventDispatcherID;
			InstancedEventDispatcherID m_nextSiblingID = InvalidInstancedEventDispatcherID;
		};

		using InstancedEventPool = ObjectPool<InstancedEventType, ObjectPoolHandle32>;
		using InstancedDispatcherPool = ObjectPool<InstancedDispatcherData, ObjectPoolHandle32>;
		using InstancedDispatcherPoolElementInfo = InstancedDispatcherPool::NewElementInfo;
		using InstancedDispatcherOrdersCollection = std::unordered_map<InstancedEventID, InstancedEventDispatcherID>;

	private:
		DispatcherPool m_dispatchers;
		DispatcherOrdersCollection m_dispatcherOrders;

		InstancedEventPool m_instancedEvents;
		InstancedDispatcherPool m_instancedDispatchers;
		InstancedDispatcherOrdersCollection m_instancedDispatcherOrders;

	public:
		EventSystem() = default;
		~EventSystem() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual bool registerEvent(EventType type) override;
		virtual bool unregisterEvent(EventType type) override;

		virtual EventDispatcherID addEventDispatcher(EventType type, const EventDispatcher& dispatcher) override;
		virtual void removeEventDispatcher(EventDispatcherID dispatcherID) override;

		virtual bool dispatchEvent(EventType type, const IEvent& event) const override;

		virtual InstancedEventID registerInstancedEvent(InstancedEventType eventType) override;
		virtual bool unregisterInstancedEvent(InstancedEventID eventID) override;

		virtual InstancedEventDispatcherID addInstanceEventDispatcher(InstancedEventID eventID, const InstancedEventDispatcher& dispatcher) override;
		virtual void removeInstancedEventDispatcher(InstancedEventDispatcherID dispatcherID) override;

		virtual bool dispatchInstancedEvent(InstancedEventID eventID, const IInstancedEvent& event) const override;
	};
}
