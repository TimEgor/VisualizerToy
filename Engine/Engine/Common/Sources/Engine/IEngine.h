#pragma once

namespace VT
{
	class EngineEnvironment;

	class IEngine
	{
	public:
		IEngine() = default;
		virtual ~IEngine() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual void run() = 0;
		virtual void updateFrame() = 0;

		virtual void startTimer() = 0;

		virtual void stop() = 0;
		virtual bool isStoped() const = 0;

		virtual void pauseRendering(bool state) = 0;
		virtual bool isRenderingPaused() const = 0;

		virtual EngineEnvironment* getEnvironment() = 0;
	};
}