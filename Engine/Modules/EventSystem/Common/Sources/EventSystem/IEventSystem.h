#pragma once

#include "IEvent.h"

#include <cstdint>
#include <functional>

namespace VT
{
	using EventDispatcher = std::function<void(const IEvent&)>;
	using EventDispatcherID = uint32_t;

	constexpr EventDispatcherID InvalidEventDispatcherID = 0;

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
	};
}
