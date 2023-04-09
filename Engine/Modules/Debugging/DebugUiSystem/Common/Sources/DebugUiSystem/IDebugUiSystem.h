#pragma once

namespace VT
{
	class IRenderContext;
	class IWindow;

	class IDebugUiSystem
	{
	public:
		IDebugUiSystem() = default;

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual bool setWindow(IWindow* window) = 0;

		virtual void update() = 0;
		virtual void render(IRenderContext* context) = 0;
	};
}
