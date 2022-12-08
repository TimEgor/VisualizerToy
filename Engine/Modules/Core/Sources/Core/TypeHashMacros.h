#pragma once

#include "HashFunctions/CRC32.h"

namespace VT
{
	class HashTyped
	{
	public:
		using Type = uint32_t;

	public:
		HashTyped() = default;
		virtual ~HashTyped() {}

		inline static constexpr Type calcType(const char* type) { return crc32(type); }
	};
}

#define VT_HASH_TYPE(TYPE_VAL, TYPE, NAME)													\
	static inline constexpr TYPE get##NAME##Type() { return calcType(TYPE_VAL); }			\
	virtual TYPE getType() const override { return get##NAME##Type(); }