#pragma once

#include "Engine/IEngine.h"
#include "Core/Clock.h"

namespace VT
{
	class Engine final : public IEngine
	{
	private:
		EngineEnvironment* m_engineEnvironment = nullptr;

		//Times
		ClockTimePoint m_startTime;
		ClockTimePoint m_currentFrameTime;
		ClockTimePoint m_prevFrameStartTime;
		float m_deltaTime = 0.0f;

		FrameID m_currentFrameID = -1;

		bool m_isStoped = false;
		bool m_isRenderingPaused = false;

	public:
		Engine() = default;
		~Engine() { release(); }

		virtual bool init(const EngineInitParam& initParam) override;
		virtual void release() override;

		virtual FrameID beginFrame() override;
		virtual void endFrame() override;
		virtual FrameID getCurrentFrameID() const override { return m_currentFrameID; }

		virtual void updateFrame() override;

		virtual void startTimer() override;

		virtual void stop() override;
		virtual bool isStoped() const override { return m_isStoped; }

		virtual void pauseRendering(bool state) override { m_isRenderingPaused = state; }
		virtual bool isRenderingPaused() const override { return m_isRenderingPaused; }

		virtual float getDeltaTime() const override { return m_deltaTime; }

		inline virtual EngineEnvironment* getEnvironment() override { return m_engineEnvironment; }
	};
}