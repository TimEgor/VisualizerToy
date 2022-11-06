#pragma once

#include "GraphicPresenter/IPresenter.h"

#include "WindowSystem/IWindowContainer.h"
#include "GraphicResourceManager/GraphicResourceHandles.h"

namespace VT
{
	class ITeture2D;

	struct SwapChainDesc;

	class WindowGraphicPresenter final : public IPresenter
	{
	private:
		WindowHandle m_windowHandle = InvalidWindowHandle;
		SwapChainHandle m_swapChainHandle = 0;

	public:
		WindowGraphicPresenter() = default;
		~WindowGraphicPresenter() { release(); }

		bool init(const char* windowTitle, const WindowSize& windowSize, const SwapChainDesc& swapChainDesc);
		void release();

		virtual uint32_t getFrameCount() const override;
		virtual ITexture2D* getTargetTexture(uint32_t frameIndex) override;
	};
}