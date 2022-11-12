#pragma once

namespace VT
{
	class ICommandList;

	class ICommandQueue
	{
	public:
		ICommandQueue() = default;
		virtual ~ICommandQueue() {}

		virtual void submitCommandList(ICommandList* commandList) = 0;
	};
}