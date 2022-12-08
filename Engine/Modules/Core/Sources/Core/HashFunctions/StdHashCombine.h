#pragma once

#include <functional>

namespace VT
{
	template <typename T>
	inline void hashCombine(std::uint16_t& seed, const T& val)
	{
		seed ^= std::hash<T>{}(val) + 0x9e37U + (seed << 3) + (seed >> 1);
	}

	template <typename T>
	inline void hashCombine(std::uint32_t& seed, const T& val)
	{
		seed ^= std::hash<T>{}(val) + 0x9e3779b9U + (seed << 6) + (seed >> 2);
	}


	template <typename T>
	inline void hashCombine(std::uint64_t& seed, const T& val)
	{
		seed ^= std::hash<T>{}(val) + 0x9e3779b97f4a7c15LLU + (seed << 12) + (seed >> 4);
	}
}