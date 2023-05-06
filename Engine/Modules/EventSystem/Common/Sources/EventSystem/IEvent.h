#pragma once

#include "Core/HashedType.h"

namespace VT
{
	using EventType = HashedType::Type;
	constexpr EventType InvalidEventType = HashedType::InvalidType;

	class IEvent : public HashedType
	{
	public:
		IEvent() = default;
		virtual ~IEvent() {}

		virtual EventType getType() const = 0;
	};

	using InstancedEventType = HashedType::Type;
	constexpr EventType InvalidInstancedEventType = HashedType::InvalidType;

	class IInstancedEvent : public HashedType
	{
	public:
		IInstancedEvent() = default;
		virtual ~IInstancedEvent() {}

		virtual InstancedEventType getType() const = 0;
	};
}

#define VT_EVENT_TYPE(EVENT_TYPE) VT_HASH_TYPE(#EVENT_TYPE, VT::EventType, Event)
#define VT_INSTANCED_EVENT_TYPE(EVENT_TYPE) VT_HASH_TYPE(#EVENT_TYPE, VT::InstancedEventType, InstancedEvents)
