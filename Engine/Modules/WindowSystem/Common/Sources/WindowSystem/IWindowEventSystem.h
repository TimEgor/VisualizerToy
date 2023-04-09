#pragma once

namespace VT
{
	class IWindowEventSystem
	{
	public:
		IWindowEventSystem() = default;
		virtual ~IWindowEventSystem() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual void updateWindowEvents() = 0;
	};
}
