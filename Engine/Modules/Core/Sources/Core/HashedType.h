#pragma once

#include "HashFunctions/CRC32.h"

namespace VT
{
	class HashedType
	{
	public:
		using Type = uint32_t;

		inline static constexpr Type InvalidType = 0;

	public:
		HashedType() = default;
		virtual ~HashedType() {}

		inline static constexpr Type calcType(const char* type) { return crc32(type); }
	};
}

#define VT_HASH_TYPE(TYPE_VAL, TYPE, NAME)													\
	static inline constexpr TYPE get##NAME##Type() { return calcType(TYPE_VAL); }			\
	virtual TYPE getType() const override { return get##NAME##Type(); }