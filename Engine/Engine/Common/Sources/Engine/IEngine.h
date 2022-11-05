#pragma once

namespace VT
{
	struct EngineEnvironment;

	struct EngineInitParam final
	{
		const char* m_platformPluginPath = nullptr;
		const char* m_graphicDevicePluginPath = nullptr;

		bool m_swapChainEnabled = true;
	};

	class IEngine
	{
	public:
		IEngine() = default;
		virtual ~IEngine() {}

		virtual bool init(const EngineInitParam& initParam) = 0;
		virtual void release() = 0;

		virtual void updateFrame() = 0;

		virtual void startTimer() = 0;

		virtual void stop() = 0;
		virtual bool isStoped() const = 0;

		virtual void pauseRendering(bool state) = 0;
		virtual bool isRenderingPaused() const = 0;

		virtual EngineEnvironment* getEnvironment() = 0;
	};
}