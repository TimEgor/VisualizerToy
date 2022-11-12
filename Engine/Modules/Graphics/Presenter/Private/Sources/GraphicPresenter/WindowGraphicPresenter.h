#pragma once

#include "GraphicPresenter/IPresenter.h"

#include "WindowSystem/IWindowContainer.h"
#include "SwapChain/ISwapChain.h"

namespace VT
{
	class ITeture2D;

	struct SwapChainDesc;

	class WindowGraphicPresenter final : public IPresenter
	{
	private:
		WindowHandle m_windowHandle = InvalidWindowHandle;
		ISwapChain* m_swapChain = nullptr;

	public:
		WindowGraphicPresenter() = default;
		~WindowGraphicPresenter() { release(); }

		bool init(const char* windowTitle, const WindowSize& windowSize, const SwapChainDesc& swapChainDesc);
		void release();

		virtual uint32_t getFrameCount() const override;
		virtual const ITexture2D* getTargetTexture(uint32_t frameIndex) const override;
	};
}