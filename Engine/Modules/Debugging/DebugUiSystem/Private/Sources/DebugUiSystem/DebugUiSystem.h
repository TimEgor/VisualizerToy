#pragma once

#include "DebugUiSystem/IDebugUiSystem.h"

namespace VT
{
	class IImGuiContext;

	class DebugUiSystem final : public IDebugUiSystem
	{
	private:
		IImGuiContext* m_context = nullptr;

	public:
		DebugUiSystem() = default;
		~DebugUiSystem() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual bool setWindow(IWindow* window) override;

		virtual void update() override;
		virtual void render(IRenderContext* context) override;
	};
}
