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

	private:
		DispatcherPool m_dispatchers;
		DispatcherOrdersCollection m_dispatcherOrders;

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
	};
}
