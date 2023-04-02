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
}

#define VT_EVENT_TYPE(EVENT_TYPE) VT_HASH_TYPE(#EVENT_TYPE, VT::EventType, Event)
