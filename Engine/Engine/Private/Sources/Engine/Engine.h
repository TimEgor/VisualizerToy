#pragma once

#include "Engine/IEngine.h"

namespace VT
{
	class Engine final : public IEngine
	{
	public:
		Engine() = default;

		virtual bool init() override;
		virtual void release() override;

		virtual void run() override;
		virtual void updateFrame() override;

		virtual void startTimer() override;

		virtual void stop() override;
		virtual bool isStoped() const override;

		virtual void pauseRendering(bool state) override;
		virtual bool isRenderingPaused() const override;

		virtual EngineEnvironment* getEnvironment() override;
	};
}