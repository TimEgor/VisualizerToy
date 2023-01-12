#pragma once

namespace VT
{
	class ICommandList
	{
	public:
		ICommandList() = default;
		virtual ~ICommandList() {}

		virtual void close() = 0;
		virtual void reset() = 0;
	};
}