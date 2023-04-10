#pragma once

#include "IEvent.h"

#include <cstdint>
#include <functional>

namespace VT
{
	using EventDispatcher = std::function<void(const IEvent&)>;
	using EventDispatcherID = uint32_t;

	using InstancedEventID = uint32_t;
	using InstancedEventDispatcher = std::function<void(const IInstancedEvent&)>;
	using InstancedEventDispatcherID = uint32_t;

	constexpr InstancedEventID InvalidInstancedEventID = 0;
	constexpr EventDispatcherID InvalidEventDispatcherID = 0;
	constexpr InstancedEventDispatcherID InvalidInstancedEventDispatcherID = 0;

	// the first quick prototype
	// TODO: need to think a more better solution for the event dispaching

	class IEventSystem
	{
	public:
		IEventSystem() = default;
		virtual ~IEventSystem() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual bool registerEvent(EventType type) = 0;
		virtual bool unregisterEvent(EventType type) = 0;

		virtual EventDispatcherID addEventDispatcher(EventType type, const EventDispatcher& dispatcher) = 0;
		virtual void removeEventDispatcher(EventDispatcherID dispatcherID) = 0;

		virtual bool dispatchEvent(EventType type, const IEvent& event) const = 0;

		virtual InstancedEventID registerInstancedEvent(InstancedEventType eventType) = 0;
		virtual bool unregisterInstancedEvent(InstancedEventID eventID) = 0;

		virtual InstancedEventDispatcherID addInstanceEventDispatcher(InstancedEventID eventID, const InstancedEventDispatcher& dispatcher) = 0;
		virtual void removeInstancedEventDispatcher(InstancedEventDispatcherID dispatcherID) = 0;

		virtual bool dispatchInstancedEvent(InstancedEventID eventID, const IInstancedEvent& event) const = 0;
	};
}
