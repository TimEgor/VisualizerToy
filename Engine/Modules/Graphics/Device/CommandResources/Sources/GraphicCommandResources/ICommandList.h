#pragma once

namespace VT
{
	class ICommandList
	{
	public:
		ICommandList() = default;
		virtual ~ICommandList() {}

		virtual void reset() = 0;
	};
}