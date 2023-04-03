#pragma once

namespace VT
{
	class IImGuiPlatformBackend
	{
	public:
		IImGuiPlatformBackend() = default;
		virtual ~IImGuiPlatformBackend() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual void newFrame() = 0;
	};

	class IImGuiRenderBackend
	{
	public:
		IImGuiRenderBackend() = default;
		virtual ~IImGuiRenderBackend() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual void newFrame() = 0;
	};
}
