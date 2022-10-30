#pragma once

namespace VT
{
	class IWindowEventSystem
	{
	public:
		IWindowEventSystem() = default;
		virtual ~IWindowEventSystem() {}

		virtual void updateWindowEvents() = 0;
	};
}