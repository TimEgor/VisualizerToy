#pragma once

#include "Core/TypeHashMacros.h"

namespace VT
{
	using EventType = HashTyped::Type;
	constexpr EventType InvalidEventType = HashTyped::InvalidType;

	class IEvent : public HashTyped
	{
	public:
		IEvent() = default;
		virtual ~IEvent() {}

		virtual EventType getType() const = 0;
	};

	using InstancedEventType = HashTyped::Type;
	constexpr EventType InvalidInstancedEventType = HashTyped::InvalidType;

	class IInstancedEvent : public HashTyped
	{
	public:
		IInstancedEvent() = default;
		virtual ~IInstancedEvent() {}

		virtual InstancedEventType getType() const = 0;
	};
}

#define VT_EVENT_TYPE(EVENT_TYPE) VT_HASH_TYPE(#EVENT_TYPE, VT::EventType, Event)
#define VT_INSTANCED_EVENT_TYPE(EVENT_TYPE) VT_HASH_TYPE(#EVENT_TYPE, VT::InstancedEventType, InstancedEvents)
