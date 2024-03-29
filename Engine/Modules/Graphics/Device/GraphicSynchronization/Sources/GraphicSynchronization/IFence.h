#pragma once

#include <cstdint>

namespace VT
{
	using FenceValueType = uint64_t;
	constexpr FenceValueType InitialFenceValue = 0;

	class IFence
	{
	public:
		IFence() = default;
		virtual ~IFence() {}

		virtual void wait(FenceValueType value) = 0;
		virtual FenceValueType getValue() const = 0;
	};
}
