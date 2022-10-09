#pragma once

namespace VT
{
	template <typename T>
	inline T getAdjustment(T size, T alignment)
	{
		return alignment - (size & (alignment - 1));
	}

	template <typename T>
	inline T align(T size, T alignment)
	{
		T adjustment = getAdjustment(size, alignment);
		if (adjustment != alignment)
		{
			return size + adjustment;
		}

		return size;
	}
}