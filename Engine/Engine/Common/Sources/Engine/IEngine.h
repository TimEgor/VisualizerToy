#pragma once

#include "Core/FileName/FileName.h"

namespace VT
{
	struct EngineEnvironment;

	struct EngineInitParam final
	{
		FileName m_platformPluginPath;
		FileName m_graphicDevicePluginPath;
		FileName m_resourceSystenPluginPath;

		FileName m_shaderConverterPath;

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