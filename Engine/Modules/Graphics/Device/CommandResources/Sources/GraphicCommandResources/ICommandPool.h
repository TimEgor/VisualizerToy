#pragma once

namespace VT
{
	class ICommandPool
	{
	public:
		ICommandPool() = default;
		virtual ~ICommandPool() {}

		virtual void reset() = 0;
	};
}