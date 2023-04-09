#pragma once

struct ImGuiContext;

namespace VT
{
	class IRenderContext;
	class IWindow;
	class IGraphicDevice;

	class IImGuiPlatformBackend
	{
	public:
		IImGuiPlatformBackend() = default;
		virtual ~IImGuiPlatformBackend() {}

		virtual bool init(ImGuiContext* imguiContext, const VT::IWindow* window) = 0;
		virtual void release() = 0;

		virtual void newFrame() = 0;
	};

	class IImGuiRenderBackend
	{
	public:
		IImGuiRenderBackend() = default;
		virtual ~IImGuiRenderBackend() {}

		virtual bool init(ImGuiContext* imguiContext) = 0;
		virtual void release() = 0;

		virtual void newFrame() = 0;

		virtual void present(VT::IRenderContext* context) = 0;
	};
}
